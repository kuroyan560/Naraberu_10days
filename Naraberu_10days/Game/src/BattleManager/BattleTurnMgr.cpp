#include "BattleTurnMgr.h"
#include "FrameWork/ImguiApp.h"
#include "FrameWork/WinApp.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

#include "FrameWork/UsersInput.h"
#include "Player_Act/Skills/PlayerSkills.h"
#include "ExistUnits.h"

#include"../OperationConfig.h"

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;
	ExistUnits::Instance()->m_NowTarget = 0;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");
}

void BattleTurnMgr::SetUnits(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;
	ExistUnits::Instance()->m_NowTarget = 0;
}

void BattleTurnMgr::OnUpdate()
{
	// 常時更新処理
	for (auto& Unit : UnitList) {
		Unit->OnAlwaysUpdate();
	}

	// 敵の生存判定
	bool EnemyAlive = false;

	// プレイヤーターン敵生存判定
	// 生きてる敵が居るか確認
	for (int i = 1; i < UnitList.size(); i++) {
		if (UnitList[i]->IsAlive()) {
			// 生きてる敵が居ればフラグを上げる
			EnemyAlive = true;
		}
	}

	// プレイヤーが死んでいる
	if (!UnitList[TurnNum]->IsAlive()) {
		// バトル終了処理(敗北)
		int a = 0;
	}

	// 生きてる敵が居ない
	if (!EnemyAlive) {
		// バトル終了処理(勝利)
		int a = 0;
	}

	// 通常の更新処理
	if (EnemyAlive && UnitList[TurnNum]->IsAlive()) {
		Update_Battle();
	}
	

	/*if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_T)) {
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
	}*/

	// ターゲット


	// スキルの更新
	PlayerSkills::PlayerSkillMgr::Instance()->Update();
}

void BattleTurnMgr::OnDraw()
{
	for (int i = 1; i < UnitList.size(); i++) {
		int EnemyIndex = i - 1;
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[i]);

		// プレイヤーターンなら暗くしない
		if (TurnNum == 0) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
		// 敵ターンの時自分のターン意外なら暗くする
		else if (i != TurnNum) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), true, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
		else {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
	}

	// 敵描画
	UnitList[TurnNum]->OnDraw();

	// プレイヤースキル描画
	PlayerSkills::PlayerSkillMgr::Instance()->Draw();

	using namespace KuroEngine;
	// カットイン中であれば
	if (CutInMgr::Instance()->NowCutIn()) {
		CutInMgr::Instance()->OnDraw();
	}
}

void BattleTurnMgr::OnImguiDebug()
{
	ImGui::Begin("GameScene");
	ImGui::Text("Naw Trun is %d", TurnNum);
	if (TurnNum == 0) {
		ImGui::Text("Type : Player");
	}
	else {
		ImGui::Text("Type : Enemy");
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[TurnNum]);
		ImGui::Text(En->GetEnemyData().m_Name.c_str());
		ImGui::Text("HP : %d", En->GetEnemyData().m_HP);
		for (auto& ActionName : En->GetEnemyData().ActionList) {
			ImGui::Text(ActionName.c_str());
		}
	}
	ImGui::End();
}

void BattleTurnMgr::NextTurnStart()
{
	TurnFrameTime = 0;
}

void BattleTurnMgr::Update_Battle()
{
	if (UnitList[TurnNum]->IsNextTurn()) {
		// ターン終了処理
		UnitList[TurnNum]->End();

		// ターン切り替え・全体ターン数加算
		// 生きてるユニットまで飛ばす
		while (1) {
			TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0, m_Whole_Turn_Count++;
			// 生きてるユニットのターン
			if (UnitList[TurnNum]->IsAlive()) {
				UnitList[TurnNum]->StartTurn();
				NextTurnStart();
				break;
			}
		}

		if (TurnNum == 0) {
			CutInMgr::Instance()->StartCutIn(CutInType::PLAYER_TURN);
		}
		else if (TurnNum == 1) {
			CutInMgr::Instance()->StartCutIn(CutInType::ENEMY_TURN);
		}
	}
	// カットイン中であれば
	if (CutInMgr::Instance()->NowCutIn()) {
		CutInMgr::Instance()->OnUpdate();
	}
	if (!CutInMgr::Instance()->NowCutIn()) {
		// FrameTime加算
		TurnFrameTime++;
		// ターン更新
		UnitList[TurnNum]->OnUpdate();
	}

	ExistUnits::Instance()->m_NowTurn = TurnNum;
	// ロックオン
	if (OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
		// 生きているユニットまで
		int iaaa = ExistUnits::Instance()->m_NowTarget;
		bool ChangeTargetSuccess = false;
		for (int i = ExistUnits::Instance()->m_NowTarget; i > 0; i--) {
			if (UnitList[i]->IsAlive()) {
				ExistUnits::Instance()->m_NowTarget = i - 1;
				ChangeTargetSuccess = true;
				break;
			}
		}
		// ターゲットを変更できなかった
		if (!ChangeTargetSuccess) {

		}
	}
	if (OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
		// 生きているユニットまで
		int iaaa = ExistUnits::Instance()->m_NowTarget;
		bool ChangeTargetSuccess = false;
		for (int i = ExistUnits::Instance()->m_NowTarget + 1; i < UnitList.size() - 1; i++) {
			if (UnitList[i + 1]->IsAlive()) {
				ExistUnits::Instance()->m_NowTarget = i;
				ChangeTargetSuccess = true;
				break;
			}
		}
		// ターゲットを変更できなかった
		if (!ChangeTargetSuccess) {

		}
	}
	// ターゲット中の敵が死んでいる場合
	if (!UnitList[ExistUnits::Instance()->m_NowTarget + 1]->IsAlive()) {
		// 一番上の生きている敵を狙う
		for (int i = 1; i < UnitList.size(); i++) {
			if (UnitList[i]->IsAlive()) {
				ExistUnits::Instance()->m_NowTarget = i - 1;
				break;
			}
		}
	}
}