#include "BattleScene.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

#include "../BattleManager/ExistUnits.h"
#include "../BattleManager/Reticle/Reticle.h"
#include "../OperationConfig.h"

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/Enemy_Act/EnemyActList.h"
#include "../BattleManager/CutIn/CutInMgr.h"
#include"../SoundConfig.h"

#include "../Effect/ScreenShakeManager.h"
#include"ForUser/Debugger.h"
#include"../Effect/ParticleManager.h"
#include"../Effect/Particle/HealParticle.h"
#include"../Effect/Particle/BackPrismParticle.h"
#include"../Effect/Particle/UltParticle.h"
#include"../Effect/ScreenShakeManager.h"

void BattleScene::OnInitialize()
{
	using namespace KuroEngine;

	// リザルト表示用
	ResultTimer = 0.0f;
	ResultTimer_Max = 80.0f * RefreshRate::RefreshRate_Mag;

	m_Stage_End = false;
	m_Impossible_Put_Block_Effect_Time = int(50.0f * RefreshRate::RefreshRate_Mag);
	m_Impossible_Put_Block_Timer = m_Impossible_Put_Block_Effect_Time;
	m_End_Timer = 0;
	m_End_Timer_Finish = int(500.0f * RefreshRate::RefreshRate_Mag);

	// ポーズ
	m_IsPause = false;
	m_PauseMenu = 0;
	m_Already_Selected_Pause = false;
	// チュートリアル
	m_NowTutorial_Step = 0;
	m_Tutoroal_Trigger_Timer = 0;

	m_Already_Selected = false;

	Pl = std::make_shared<Player>();
	Pl->OnInitialize();
	Pl->StartTurn();
	UltParticle::SetPlayer(Pl);

	// レティクルが動くように
	Reticle::Instance()->m_CanMove = true;

	// ステージをセット
	SetStage(ExistUnits::Instance()->m_StageName);
	// 最初のウェーブ
	m_NowWave = 1;
	// ウェーブの敵を取得
	std::vector<EnemysData::EnemyData> EnemyData = m_NowStage.GetWaveEnemyIndex(m_NowWave);
	for (auto& data : EnemyData) {
		En.emplace_back(std::make_shared<Enemy>());
		GetUnitPtr<Enemy>(En.back())->SetEnemyData(data);
	}
	// データをセット
	Mgr.OnInitialize(Pl, En);
	if (En.size() == 1) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get());
	}
	if (En.size() == 2) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get());
	}
	if (En.size() == 3) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());
	}

	stage.reset(new PanelManager());
	stage->Initialize();
	block.reset(new BlockManager());
	//中央指定
	block->SetCenter(stage->GetCenter());
	block->Initialize();
	block->SetTutorial();

	ExistUnits::Instance()->SetMapChipPtr(stage->GetMapChipPtr());
	ExistUnits::Instance()->SetStageManagerPtr(stage.get());

	//演出初期化
	m_setPrismEffect->Init();
	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx]->Init();
	ScreenShakeManager::Instance()->Init();

	dame.reset(new PlayerDamageUi());
	dame->Initialize();

	ultAttackEffect.reset(new UltAttackEffect());
	ultAttackEffect->Initialize();


	SoundConfig::Instance()->SwitchBGM(SoundConfig::BGM_MAIN);

	Debugger::Register({
	SoundConfig::Instance(),
		});

	ParticleManager::Instance()->Init();
}

void BattleScene::OnUpdate()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	static int mmmm = 0;

	if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_J))
	{
		ScreenShakeManager::Instance()->Shake();
	}

	//ダメージ表記（移動する）
	{
		if (OperationConfig::Instance()->DebugKeyInput(DIK_H)) {
			dame->SetDamage(123);
			ultAttackEffect->SetUltAttack();
		}
		dame->Update();
		ultAttackEffect->Update();
	}

	if (OperationConfig::Instance()->DebugKeyInput(DIK_L) && mmmm % 3 == 0)
	{
		auto randPos = KuroEngine::GetRand(KuroEngine::Vec2<float>(-240.0f, -240.0f), KuroEngine::Vec2<float>(240.0f, 240.0f));
		m_healPtEmitter->Emit(KuroEngine::Vec2<float>(640.0f, 360.0f) + randPos, 1);
	}
	if (OperationConfig::Instance()->DebugKeyInput(DIK_K) && mmmm % 2 == 0)
	{
		auto randPos = KuroEngine::GetRand(KuroEngine::Vec2<float>(-240.0f, -240.0f), KuroEngine::Vec2<float>(240.0f, 240.0f));
		m_backPrismPtEmitter->Emit(KuroEngine::Vec2<float>(640.0f, 360.0f) + randPos, 1);
	}

	mmmm++;

	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::MENU_IN_GAME,OperationConfig::ON_TRIGGER)) {
		m_IsPause = true;
		m_PauseMenu = 0;
		// 現在時刻
		GetLocalTime(&Mgr.PauseStartTime);
		// ポーズ時間の計算
		Mgr.m_PauseTimeContainer.emplace_back(Mgr.m_PauseTime);
	}

	if (m_IsPause) {
		// 現在時刻
		GetLocalTime(&Mgr.PauseEndTime);
		// 変換
		FILETIME ftime1;
		FILETIME ftime2;
		SystemTimeToFileTime(&Mgr.PauseStartTime, &ftime1);
		SystemTimeToFileTime(&Mgr.PauseEndTime, &ftime2);
		// int64にキャスト
		__int64* nTime1 = (__int64*)&ftime1;
		__int64* nTime2 = (__int64*)&ftime2;
		// 経過秒
		Mgr.m_PauseTime = (*nTime2 - *nTime1);

		if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP) ||
			OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
			if (m_PauseMenu > 0) {
				m_PauseMenu--;
				SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
			}
		}
		if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN) ||
			OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
			if (m_PauseMenu < 2) {
				m_PauseMenu++;
				SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
			}
		}
		// 決定
		if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
			m_Already_Selected_Pause = true;
			SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
			// 戻る
			if (m_PauseMenu == 0) {
				m_Already_Selected_Pause = false;
				m_IsPause = false;
			}
			// タイトル
			else if (m_PauseMenu == 1) {
				KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle", &m_Fade);
			}
			// リトライ
			else if (m_PauseMenu == 2) {
				ExistUnits::Instance()->m_ChangeStageSelect = true;
				KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title", &m_Fade);
			}
		}
		return;
	}

	// ウェーブ終了・次ウェーブスタート
	if (Mgr.ChangeNextWave()) {
		// カットイン
		// 現在が最後のウェーブではなかった場合
		if (!(m_NowWave + 1 > m_NowStage.m_Stage_Wave_Count)) {
			CutInMgr::Instance()->StartCutIn(CutInType::NEXT_BATTLE);
		}
		NextWave();
	}

	// ステージ終了(敗北)
	if (Mgr.GetDefeat()) {
		ResultTimer += 1.0f;
		if (m_End_Timer < m_End_Timer_Finish) {
			m_End_Timer++;
		}
		// レティクルを動かないように
		Reticle::Instance()->m_CanMove = false;

		// 選択肢
		if (!m_Already_Selected) {
			if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP) || 
				OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
				if (m_GameOverSelectIndex > 0) {
					m_GameOverSelectIndex--;
					SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
				}
			}
			if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN) || 
				OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
				if (m_GameOverSelectIndex < 1) {
					m_GameOverSelectIndex++;
					SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
				}
			}
			// 決定
			if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)
				&& m_End_Timer > 1 && m_End_Timer >= int(float(m_End_Timer_Finish) / 2.0f)) {
				m_Already_Selected = true;
				SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
				// タイトルへ
				if (m_GameOverSelectIndex == 1) {
					ExistUnits::Instance()->m_ChangeStageSelect = true;
					KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title", &m_Fade);
				}
				// リトライ
				else if (m_GameOverSelectIndex == 0) {
					KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle", &m_Fade);
				}
			}
		}
	}
	// ステージ終了(敵全滅)
	else if (m_Stage_End) {
		ResultTimer += 1.0f;
		if (m_End_Timer < m_End_Timer_Finish) {
			m_End_Timer++;
		}
		// レティクルを動かないように
		Reticle::Instance()->m_CanMove = false;

		if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)
			&& m_End_Timer > 1 && m_End_Timer >= int(float(m_End_Timer_Finish) / 2.0f)) {
			ExistUnits::Instance()->m_ChangeStageSelect = true;
			KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title", &m_Fade);
			SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
		}
	}

	//セット可能ならセットする
	if (ExistUnits::Instance()->m_NowTurn == 0 && m_Impossible_Put_Block_Timer == 0 && Mgr.AliveEnemys() && !ExistUnits::Instance()->m_pPlayer->m_IsEndTurnFunc
		&& !Mgr.GetSelectedTurnEnd()) {
		PlayerTurn();
	}

	// 設置不可時間の更新
	if (m_Impossible_Put_Block_Timer > 0) {
		m_Impossible_Put_Block_Timer--;
	}

	Mgr.OnUpdate();
	stage->Update(KuroEngine::GetWeakPtrArray(m_enemyDamageUI));
	if (!Mgr.GetDefeat() && !m_Stage_End) {
		block->Update();
	}

	if (Mgr.GetNowTurn() == 1) {
		block->ResetPass();
	}

	//演出更新
	m_setPrismEffect->Update(stage, m_ultPtEmitter);
	for (auto& ui : m_enemyDamageUI)ui->Update(m_ultPtEmitter);
	ParticleManager::Instance()->Update();
	ScreenShakeManager::Instance()->Update();
}

void BattleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});

	// 背景色
	DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f)
		, WinApp::Instance()->GetExpandWinSize(), Color(50, 49, 59, 255), true);

	ParticleManager::Instance()->BackDraw();


	// Vキーを押してる間だけ透かしを描画
	/*if (UsersInput::Instance()->KeyInput(DIK_V)) {
		DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_SukasiTex);
	}*/
	// 背景描画
	DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f) + ScreenShakeManager::Instance()->GetOffset()
		, WinApp::Instance()->GetExpandWinSize() + ScreenShakeManager::Instance()->GetOffset(), m_BackTex);
	DrawFunc2D::DrawGraph(Vec2(881.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), m_StageTex);
	DrawFunc2D::DrawGraph(Vec2(1064.0f, 13.0f) + ScreenShakeManager::Instance()->GetOffset(), m_BattleTex);

	// ステージ名描画
	if (ExistUnits::Instance()->m_StageName == "Stage1") {
		DrawFunc2D::DrawNumber2D(1, Vec2(986.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
		DrawFunc2D::DrawGraph(Vec2(1003.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), m_NumberTex[10]);
		DrawFunc2D::DrawNumber2D(1, Vec2(1022.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}
	else if (ExistUnits::Instance()->m_StageName == "Stage2") {
		DrawFunc2D::DrawNumber2D(1, Vec2(986.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
		DrawFunc2D::DrawGraph(Vec2(1003.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), m_NumberTex[10]);
		DrawFunc2D::DrawNumber2D(2, Vec2(1022.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}
	else if (ExistUnits::Instance()->m_StageName == "Stage3") {
		DrawFunc2D::DrawNumber2D(1, Vec2(986.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
		DrawFunc2D::DrawGraph(Vec2(1003.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), m_NumberTex[10]);
		DrawFunc2D::DrawNumber2D(3, Vec2(1022.0f, 19.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}

	// ウェーブ数描画
	DrawFunc2D::DrawNumber2D(m_NowWave > m_NowStage.m_Stage_Wave_Count ? m_NowStage.m_Stage_Wave_Count : m_NowWave,
		Vec2(1201.0f, 13.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex_Battle.front());
	DrawFunc2D::DrawGraph(Vec2(1221.0f, 13.0f) + ScreenShakeManager::Instance()->GetOffset(), m_NumberTex_Battle[10]);
	DrawFunc2D::DrawNumber2D(m_NowStage.m_Stage_Wave_Count, Vec2(1246.0f, 13.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex_Battle.front());

	// プレイヤーはバトル中以外でも描画する為こっち
	Pl->OnDraw();

	Mgr.AutoTurnEndTimerDraw();

	stage->Draw();
	block->Draw();

	Mgr.OnDraw();

	// キー描画
	if (OperationConfig::Instance()->GetLatestDevice() == OperationConfig::Instance()->KEY_BOARD_MOUSE) {
		DrawFunc2D::DrawGraph(Vec2(320.0f, 90.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Ult[0]);
		DrawFunc2D::DrawGraph(Vec2(718.0f, 0.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Set[0]);
		DrawFunc2D::DrawGraph(Vec2(105.0f, 621.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Pass[0]);
		DrawFunc2D::DrawGraph(Vec2(374.0f, 655.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Left[0]);
		DrawFunc2D::DrawGraph(Vec2(587.0f, 655.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Right[0]);
	}
	else {
		DrawFunc2D::DrawGraph(Vec2(322.0f, 92.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Ult[1]);
		DrawFunc2D::DrawGraph(Vec2(719.0f, 1.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Set[1]);
		DrawFunc2D::DrawGraph(Vec2(108.0f, 623.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Pass[1]);
		DrawFunc2D::DrawGraph(Vec2(357.0f, 670.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Left[1]);
		DrawFunc2D::DrawGraph(Vec2(578.0f, 670.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Operation_Right[1]);
	}

	//演出
	ParticleManager::Instance()->FrontDraw();
	m_setPrismEffect->Draw();
	for (auto& ui : m_enemyDamageUI)ui->Draw();

	// ステージ終了(敗北)
	if (Mgr.GetDefeat()) {
		GameOverDraw();
	}
	// ステージ終了(敵全滅)
	else if (m_Stage_End) {
		GameClearDraw();
	}

	// Pause
	if (m_IsPause) {
		PauseDraw();
	}

	dame->Draw();
	ultAttackEffect->Draw();
}

void BattleScene::OnImguiDebug()
{
	Mgr.OnImguiDebug();

	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();
	using namespace KuroEngine;

	Vec2<float> mousePos = { 0.0f,0.0f };
	mousePos = input->GetMousePos();

	ImGui::Begin("attribute");
	if (attribute == BlockAttribute::attack1) {
		ImGui::Text("attack1");
	}else if (attribute == BlockAttribute::attack2) {
		ImGui::Text("attack2");
	}if (attribute == BlockAttribute::recovery) {
		ImGui::Text("recovery");
	}
	ImGui::End();

	KuroEngine::Debugger::Draw();
}

void BattleScene::OnFinalize()
{
	// リザルト表示用
	ResultTimer = 0.0f;

	En.clear();
	PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
	EnemyActions::EnemyActionMgr::Instance()->AllClear();

	KuroEngine::Debugger::ClearRegister();
}

void BattleScene::SetStage(std::string StageName)
{
	m_NowStage = Stages::StageList::Instance()->GetStage(StageName);
}

void BattleScene::NextWave()
{
	// 次ウェーブに変更
	m_NowWave++;
	m_Impossible_Put_Block_Timer = m_Impossible_Put_Block_Effect_Time;

	// 現在が最後のウェーブだった場合
	if (m_NowWave > m_NowStage.m_Stage_Wave_Count) {
		if (!m_Stage_End) {
			m_Stage_End = true;
			// 攻撃等をクリア
			PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
			EnemyActions::EnemyActionMgr::Instance()->AllClear();
			SoundConfig::Instance()->Play(SoundConfig::SE_STAGE_CLEAR, 35);
		}
		return;
	}

	// パネルを掃除
	//ExistUnits::Instance()->m_StageManager->Reset();

	// 次ウェーブの敵を取得
	En.clear();
	std::vector<EnemysData::EnemyData> EnemyData = m_NowStage.GetWaveEnemyIndex(m_NowWave);
	for (auto& data : EnemyData) {
		En.emplace_back(std::make_shared<Enemy>());
		GetUnitPtr<Enemy>(En.back())->SetEnemyData(data);
	}
	Mgr.SetUnits(Pl, En);
	// パスのリセット
	block->ResetPass();
	if (En.size() == 1) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get());
	}
	if (En.size() == 2) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get());
	}
	if (En.size() == 3) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());
	}

	// 攻撃等をクリア
	PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
	EnemyActions::EnemyActionMgr::Instance()->AllClear();
}

void BattleScene::GameClearDraw()
{
	using namespace KuroEngine;

	Vec2 Value = Vec2(0.0f, ResultEasing(ResultTimer / ResultTimer_Max)) * 450.0f;
	float SubFrame = 6.6f * RefreshRate::RefreshRate_Mag;

	Vec2 Panel_LT = Vec2(391.0f, 67.0f);
	Vec2 Panel_RB = Vec2(899.0f, 564.0f);

	// 後ろを暗く
	float alpha = 0.4f * (ResultTimer / ResultTimer_Max);
	DrawFunc2D::DrawBox2D(Panel_LT, Panel_RB, Color(0.1f, 0.1f, 0.1f, alpha > 0.4f ? 0.4f : alpha), true);

	// リザルト描画
	DrawFunc2D_Mask::DrawGraph(Vec2(503.0f, 175.0f) + Value, m_ClearTex, Panel_LT, Panel_RB);
	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame) / ResultTimer_Max))) * 450.0f;

	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame * 2.0f) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(484.0f, 257.0f) + Value, m_MaxComboTex, Panel_LT, Panel_RB);
	if (ResultTimer >= ResultTimer_Max + 30 * RefreshRate::RefreshRate_Mag) {
		DrawFunc2D::DrawNumber2D(ExistUnits::Instance()->m_MaxBonusCount, Vec2(697.0f, 257.0f) + Value, &m_ClearNumberTex.front());
	}

	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame * 3.0f) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(489.0f, 308.0f) + Value, m_TotalTurnTex, Panel_LT, Panel_RB);
	if (ResultTimer >= ResultTimer_Max + 60 * RefreshRate::RefreshRate_Mag) {
		DrawFunc2D::DrawNumber2D(Mgr.GetTotalTurn(), Vec2(697.0f, 308.0f) + Value, &m_ClearNumberTex.front());
	}

	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame * 4.0f) / ResultTimer_Max))) * 450.0f;
	if (OperationConfig::Instance()->GetLatestDevice() == OperationConfig::Instance()->KEY_BOARD_MOUSE) {
		DrawFunc2D_Mask::DrawGraph(Vec2(613.0f, 365.0f) + Value, m_Done_KeyTex_Clear, Panel_LT, Panel_RB);
	}
	else {
		DrawFunc2D_Mask::DrawGraph(Vec2(611.0f, 365.0f) + Value, m_Done_ControllerTex_Clear, Panel_LT, Panel_RB);
	}
	DrawFunc2D_Mask::DrawGraph(Vec2(613.0f, 423.0f) + Value, m_DoneTex_Clear, Panel_LT, Panel_RB);
}

void BattleScene::GameOverDraw()
{
	using namespace KuroEngine;
	Vec2 Value = Vec2(0.0f, ResultEasing(ResultTimer / ResultTimer_Max)) * 450.0f;
	float SubFrame = 6.6f * RefreshRate::RefreshRate_Mag;

	Vec2 Panel_LT = Vec2(391.0f, 67.0f);
	Vec2 Panel_RB = Vec2(899.0f, 564.0f);

	// 後ろを暗く
	float alpha = 0.4f * (ResultTimer / ResultTimer_Max);
	DrawFunc2D::DrawBox2D(Panel_LT, Panel_RB, Color(0.1f, 0.1f, 0.1f, alpha > 0.4f ? 0.4f : alpha), true);

	// リザルト描画
	DrawFunc2D_Mask::DrawGraph(Vec2(524.0f, 175.0f) + Value, m_GameoverTex, Panel_LT, Panel_RB);
	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(594.0f, 254.0f) + Value, m_RetryTex, Panel_LT, Panel_RB);
	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame * 2.0f) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(541.0f, 309.0f) + Value, m_StageSelectTex, Panel_LT, Panel_RB);

	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame) / ResultTimer_Max))) * 450.0f;
	if (m_GameOverSelectIndex == 0) {
		DrawFunc2D_Mask::DrawGraph(Vec2(565.0f, 254.0f) + Value, m_SelectTex, Panel_LT, Panel_RB);
	}
	else {
		DrawFunc2D_Mask::DrawGraph(Vec2(512.0f, 309.0f) + Value, m_SelectTex, Panel_LT, Panel_RB);
	}

	Value = Vec2(0.0f, ResultEasing(((ResultTimer - SubFrame * 3.0f) / ResultTimer_Max))) * 450.0f;
	if (OperationConfig::Instance()->GetLatestDevice() == OperationConfig::Instance()->KEY_BOARD_MOUSE) {
		DrawFunc2D_Mask::DrawExtendGraph2D(Vec2(772.0f, 350.0f) + Value, Vec2(813.0f, 395.0f) + Value, m_Done_KeyTex_GameOver, Panel_LT, Panel_RB);
	}
	else {
		DrawFunc2D_Mask::DrawExtendGraph2D(Vec2(770.0f, 350.0f) + Value, Vec2(813.0f, 395.0f) + Value, m_Done_ControllerTex_GameOver, Panel_LT, Panel_RB);
	}
	DrawFunc2D_Mask::DrawGraph(Vec2(764.0f, 399.0f) + Value, m_DoneTex_GameOver, Panel_LT, Panel_RB);
}

void BattleScene::PauseDraw()
{
	using namespace KuroEngine;
	// 後ろを暗く
	DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f)
		, WinApp::Instance()->GetExpandWinSize(), Color(0.1f, 0.1f, 0.1f, 0.4f), true);

	Vec2 Value = Vec2(0.0f, ResultEasing(ResultTimer_Max / ResultTimer_Max)) * 450.0f;
	float SubFrame = 0.0f * RefreshRate::RefreshRate_Mag;

	Vec2 Panel_LT = Vec2(391.0f, 67.0f);
	Vec2 Panel_RB = Vec2(899.0f, 564.0f);

	// 後ろを暗く
	//float alpha = 0.4f * (ResultTimer / ResultTimer_Max);
	//DrawFunc2D::DrawBox2D(Panel_LT, Panel_RB, Color(0.1f, 0.1f, 0.1f, alpha > 0.4f ? 0.4f : alpha), true);

	// リザルト描画
	DrawFunc2D_Mask::DrawGraph(Vec2(570.0f, 173.0f) + Value, m_PauseTex, Panel_LT, Panel_RB);
	Value = Vec2(0.0f, ResultEasing(((ResultTimer_Max - SubFrame) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(578.0f, 254.0f) + Value, m_ResumeTex_Pause, Panel_LT, Panel_RB);
	Value = Vec2(0.0f, ResultEasing(((ResultTimer_Max - SubFrame * 2.0f) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(594.0f, 310.0f) + Value, m_RetryTex_Pause, Panel_LT, Panel_RB);
	Value = Vec2(0.0f, ResultEasing(((ResultTimer_Max - SubFrame * 3.0f) / ResultTimer_Max))) * 450.0f;
	DrawFunc2D_Mask::DrawGraph(Vec2(541.0f, 365.0f) + Value, m_StageSelectTex_Pause, Panel_LT, Panel_RB);

	Value = Vec2(0.0f, ResultEasing(((ResultTimer_Max - SubFrame) / ResultTimer_Max))) * 450.0f;
	if (m_PauseMenu == 0) {
		DrawFunc2D_Mask::DrawGraph(Vec2(552.0f, 254.0f) + Value, m_SelectCosorTex_Pause, Panel_LT, Panel_RB);
	}
	else if(m_PauseMenu == 1) {
		DrawFunc2D_Mask::DrawGraph(Vec2(568.0f, 310.0f) + Value, m_SelectCosorTex_Pause, Panel_LT, Panel_RB);
	}
	else {
	DrawFunc2D_Mask::DrawGraph(Vec2(515.0f, 365.0f) + Value, m_SelectCosorTex_Pause, Panel_LT, Panel_RB);
	}

	Value = Vec2(0.0f, ResultEasing(((ResultTimer_Max - SubFrame * 3.0f) / ResultTimer_Max))) * 450.0f;
	if (OperationConfig::Instance()->GetLatestDevice() == OperationConfig::Instance()->KEY_BOARD_MOUSE) {
		DrawFunc2D_Mask::DrawExtendGraph2D(Vec2(767.0f, 399.0f) + Value, Vec2(817.0f, 450.0f) + Value, m_Done_KeyTex_Pause, Panel_LT, Panel_RB);
	}
	else {
		DrawFunc2D_Mask::DrawExtendGraph2D(Vec2(770.0f, 403.0f) + Value, Vec2(813.0f, 448.0f) + Value, m_Done_ControllerTex_Pause, Panel_LT, Panel_RB);
	}
	DrawFunc2D_Mask::DrawGraph(Vec2(764.0f, 452.0f) + Value, m_DoneTex_Pause, Panel_LT, Panel_RB);
}

float BattleScene::ResultEasing(float time)
{
	float t = time;
	if (t > 1.0f) {
		t = 1.0f;
	}
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	float Ret = t < 0.5f
		? (powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	return 1.0f - Ret;
}

void BattleScene::TutorialUpdate()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::MENU_IN_GAME, OperationConfig::ON_TRIGGER)) {
		m_IsPause = true;
		m_PauseMenu = 0;
		// 現在時刻
		GetLocalTime(&Mgr.PauseStartTime);
		// ポーズ時間の計算
		Mgr.m_PauseTimeContainer.emplace_back(Mgr.m_PauseTime);
	}

	if (m_IsPause) {
		// 現在時刻
		GetLocalTime(&Mgr.PauseEndTime);
		// 変換
		FILETIME ftime1;
		FILETIME ftime2;
		SystemTimeToFileTime(&Mgr.PauseStartTime, &ftime1);
		SystemTimeToFileTime(&Mgr.PauseEndTime, &ftime2);
		// int64にキャスト
		__int64* nTime1 = (__int64*)&ftime1;
		__int64* nTime2 = (__int64*)&ftime2;
		// 経過秒
		Mgr.m_PauseTime = (*nTime2 - *nTime1);

		if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP) ||
			OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
			if (m_PauseMenu > 0) {
				m_PauseMenu--;
				SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
			}
		}
		if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN) ||
			OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
			if (m_PauseMenu < 2) {
				m_PauseMenu++;
				SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
			}
		}
		// 決定
		if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
			m_Already_Selected_Pause = true;
			SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
			// 戻る
			if (m_PauseMenu == 0) {
				m_Already_Selected_Pause = false;
				m_IsPause = false;
			}
			// タイトル
			else if (m_PauseMenu == 1) {
				KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle", &m_Fade);
			}
			// リトライ
			else if (m_PauseMenu == 2) {
				ExistUnits::Instance()->m_ChangeStageSelect = true;
				KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title", &m_Fade);
			}
		}
		return;
	}

	// タイマー増加
	m_Tutoroal_Trigger_Timer++;
	// チュートリアル説明中
	if (m_Tutorial_Pause[m_NowTutorial_Step] == true) {
		// 決定
		if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
			m_NowTutorial_Step++;

			if (m_NowTutorial_Step == 7) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else if (m_NowTutorial_Step == 11) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else if (m_NowTutorial_Step == 15) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else if (m_NowTutorial_Step == 19) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else if (m_NowTutorial_Step == 25) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else if (m_NowTutorial_Step == 27) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else if (m_NowTutorial_Step == 33) {
				// 実践
				m_Tutorial_Pause[m_NowTutorial_Step] = false;
			}
			else {
				m_Tutorial_Pause[m_NowTutorial_Step] = true;
			}
		}

		//演出更新
		m_setPrismEffect->Update(stage, m_ultPtEmitter);
		for (auto& ui : m_enemyDamageUI)ui->Update(m_ultPtEmitter);
		ParticleManager::Instance()->Update();
		ScreenShakeManager::Instance()->Update();

		return;
	}

	// ウェーブ終了・次ウェーブスタート
	if (Mgr.ChangeNextWave()) {
		// カットイン
		// 現在が最後のウェーブではなかった場合
		if (!(m_NowWave + 1 > m_NowStage.m_Stage_Wave_Count)) {
			CutInMgr::Instance()->StartCutIn(CutInType::NEXT_BATTLE);
		}
		NextWave();
	}

	// ステージ終了(敗北)
	if (Mgr.GetDefeat()) {
		ResultTimer += 1.0f;
		if (m_End_Timer < m_End_Timer_Finish) {
			m_End_Timer++;
		}
		// レティクルを動かないように
		Reticle::Instance()->m_CanMove = false;

		// 選択肢
		if (!m_Already_Selected) {
			if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP) ||
				OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
				if (m_GameOverSelectIndex > 0) {
					m_GameOverSelectIndex--;
					SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
				}
			}
			if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN) ||
				OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
				if (m_GameOverSelectIndex < 1) {
					m_GameOverSelectIndex++;
					SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
				}
			}
			// 決定
			if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)
				&& m_End_Timer > 1 && m_End_Timer >= int(float(m_End_Timer_Finish) / 2.0f)) {
				m_Already_Selected = true;
				SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
				// タイトルへ
				if (m_GameOverSelectIndex == 1) {
					ExistUnits::Instance()->m_ChangeStageSelect = true;
					KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title", &m_Fade);
				}
				// リトライ
				else if (m_GameOverSelectIndex == 0) {
					KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle", &m_Fade);
				}
			}
		}
	}
	// ステージ終了(敵全滅)
	else if (m_Stage_End) {
		ResultTimer += 1.0f;
		if (m_End_Timer < m_End_Timer_Finish) {
			m_End_Timer++;
		}
		// レティクルを動かないように
		Reticle::Instance()->m_CanMove = false;

		if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)
			&& m_End_Timer > 1 && m_End_Timer >= int(float(m_End_Timer_Finish) / 2.0f)) {
			ExistUnits::Instance()->m_ChangeStageSelect = true;
			KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title", &m_Fade);
			SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
		}
	}

	//セット可能ならセットする
	if (ExistUnits::Instance()->m_NowTurn == 0 && m_Impossible_Put_Block_Timer == 0 && Mgr.AliveEnemys() && !ExistUnits::Instance()->m_pPlayer->m_IsEndTurnFunc
		&& !Mgr.GetSelectedTurnEnd()) {
		PlayerTurn();
	}

	// 設置不可時間の更新
	if (m_Impossible_Put_Block_Timer > 0) {
		m_Impossible_Put_Block_Timer--;
	}

	Mgr.OnUpdate();
	stage->Update(KuroEngine::GetWeakPtrArray(m_enemyDamageUI));
	if (!Mgr.GetDefeat() && !m_Stage_End) {
		block->Update();
	}

	if (Mgr.GetNowTurn() == 1) {
		block->ResetPass();
	}

	//演出更新
	m_setPrismEffect->Update(stage, m_ultPtEmitter);
	for (auto& ui : m_enemyDamageUI)ui->Update(m_ultPtEmitter);
	ParticleManager::Instance()->Update();
	ScreenShakeManager::Instance()->Update();
}

BattleScene::BattleScene()
{
	// ---- 背景
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_SukasiTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "Sukasi.png");
	m_BackTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "battle_scene_frame.png");
	m_StageTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/info/stage.png");
	m_BattleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/info/battle.png");
	D3D12App::Instance()->GenerateTextureBuffer(&m_NumberTex.front(), TexDir + "/info/stage_number.png", 12, Vec2(12, 1));
	D3D12App::Instance()->GenerateTextureBuffer(&m_NumberTex_Battle.front(), TexDir + "/info/battle_number.png", 11, Vec2(11, 1));

	// ステージクリア
	m_ClearTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/stage_clear.png");
	m_MaxComboTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/max_combo.png");
	m_TotalTurnTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/total_turn.png");
	m_Done_KeyTex_Clear = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/done_key.png");
	m_Done_ControllerTex_Clear = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/done_controller.png");
	m_DoneTex_Clear = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/done.png");
	D3D12App::Instance()->GenerateTextureBuffer(&m_ClearNumberTex.front(), TexDir + "/clear/clear_number.png", 10, Vec2(10, 1));

	// ゲームオーバー
	m_GameOverSelectIndex = 0;
	m_Already_Selected = false;
	m_GameoverTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/game_over.png");
	m_RetryTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/retry.png");
	m_StageSelectTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/stage_select.png");
	m_SelectTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/select.png");
	m_Done_KeyTex_GameOver = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/done_key.png");
	m_Done_ControllerTex_GameOver = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/done_controller.png");
	m_DoneTex_GameOver = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/gameover/done.png");

	// ポーズ
	m_PauseTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/pause.png");
	m_ResumeTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/resume.png");
	m_RetryTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/retry.png");
	m_StageSelectTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/stage_select.png");
	m_SelectCosorTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/select.png");
	m_Done_KeyTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/done_key.png");
	m_Done_ControllerTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/done_controller.png");
	m_DoneTex_Pause = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/pause/done.png");

	// 操作キー
	for (int i = 0; i < 2; i++) {
		std::string TexDir_Opr;
		if (i == 0) TexDir_Opr = "resource/user/tex/battle_scene/operation/key/";
		else if (i == 1)TexDir_Opr = "resource/user/tex/battle_scene/operation/controller/";
		m_Operation_Done[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "done.png");
		m_Operation_Pass[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "pass.png");
		m_Operation_Left[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "select_left_prism.png");
		m_Operation_Right[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "select_right_prism.png");
		m_Operation_Set[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "set_prism.png");
		m_Operation_TurnEnd[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "turn_end.png");
		m_Operation_Ult[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir_Opr + "ult.png");
	}

	//敵のダメージUIの位置
	const std::array<Vec2<float>, ENEMY_COUNT_MAX>ENEMY_DAMAGE_UI_POS =
	{
		Vec2<float>(949.0f,204.0f),
		Vec2<float>(949.0f,374.0f),
		Vec2<float>(949.0f,544.0f),
	};

	//敵のダメージUIの位置設定
	std::vector<std::weak_ptr<SkillResultUI>>enemyDamageUiWeakPtr(ENEMY_COUNT_MAX);
	m_enemyDamageUI.resize(ENEMY_COUNT_MAX);
	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
	{
		m_enemyDamageUI[enemyIdx] = std::make_shared<SkillResultUI>();
		m_enemyDamageUI[enemyIdx]->Set(SkillResultUI::SKILL_ENEMY_DAMAGE, ENEMY_DAMAGE_UI_POS[enemyIdx], 1500.0f);
		enemyDamageUiWeakPtr[enemyIdx] = m_enemyDamageUI[enemyIdx];
	}

	m_setPrismEffect = std::make_shared<SetPrismEffect>(enemyDamageUiWeakPtr);

	m_healPtEmitter = ParticleManager::Instance()->Register<HealParticle>(1000, false);
	m_backPrismPtEmitter = ParticleManager::Instance()->Register<BackPrismParticle>(1000, true);
	m_ultPtEmitter = ParticleManager::Instance()->Register<UltParticle>(1000, false);
}

void BattleScene::PlayerTurn()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::SET_PRISM, OperationConfig::ON_TRIGGER)) {
		KuroEngine::Vec2<int> nowMapchip;
		std::vector<KuroEngine::Vec2<int>> shape;
		BlockColor color;

		//ブロック情報取得
		block->GetBlock(&nowMapchip, &shape, &attribute, &color);
		//配置可能なら配置する
		//if (!stage->JudgeSet(nowMapchip, shape, attribute, color)) { return; }
		if (!stage->JudgeWithEffect(nowMapchip, shape, attribute, color, m_setPrismEffect)) { return; }
		//次の使用ブロックをセットする
		block->ChangeBlock();
	}
}
