#include "BattleTurnMgr.h"
#include"../../src/engine/FrameWork/ImguiApp.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "Player.h"
#include "Enemy.h"

void BattleTurnMgr::StartCutIn(int CutInNum)
{
	CutInTimer = 1;
}

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	TurnNum = 0;
	TurnFrameTime = 0;

	NowEmptyTurn = false;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");
}

void BattleTurnMgr::OnUpdate()
{
	// 常時更新処理
	for (auto& Unit : UnitList) {
		Unit->OnAlwaysUpdate();
	}

	if (UnitList[TurnNum]->IsNextTurn()) {
		// ターン終了処理
		UnitList[TurnNum]->End();

		// ターン切り替え
		TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0;

		// ターン開始処理
		UnitList[TurnNum]->StartTurn();
		NextTurnStart();
		StartCutIn(0);
	}
	// カットイン中であれば
	if (NowCutIn()) {
		CutInTimer < CutInEndTime ? CutInTimer++ : CutInTimer = 0;
	}
	if (!NowCutIn()) {
		// FrameTime加算
		TurnFrameTime++;
		// ターン更新
		UnitList[TurnNum]->OnUpdate();
	}
}

void BattleTurnMgr::OnDraw()
{
	for (int i = 1; i < UnitList.size(); i++) {
		int EnemyIndex = i - 1;
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[i]);
		En->Draw(EnemyIndex);
	}

	using namespace KuroEngine;
	// カットイン中であれば
	if (NowCutIn()) {
		float Window_Width = WinApp::Instance()->GetExpandWinSize().x;
		float Tex_Width = 558.0f;

		float ease_value = OutInQuart(float(CutInTimer), 0.0f, 1.0f, float(CutInEndTime));

		float Pos = (Window_Width + Tex_Width * 2) - (Window_Width + Tex_Width * 4) * ease_value;

		// 縦
		float Height_Box = 413.0f - 307.0f;
		float Box_Center = 307.0f + Height_Box / 2.0f;
		float Height_Scale_Box = Height_Box * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);

		float Height_Moji = 400.0f - 319.0f;
		float Moji_Center = 319.0f + Height_Moji / 2.0f;
		float Height_Scale_Moji = Height_Moji * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);

		DrawFunc2D::DrawBox2D(Vec2(0.0f, Box_Center - Height_Scale_Box), Vec2(Window_Width, Box_Center + Height_Scale_Box), Color(29, 29, 35, 255), true);
		DrawFunc2D::DrawExtendGraph2D(Vec2(Pos - Tex_Width / 2.0f, Moji_Center - Height_Scale_Moji), Vec2(Pos + Tex_Width / 2.0f, Moji_Center + Height_Scale_Moji), m_CutInTex);
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
