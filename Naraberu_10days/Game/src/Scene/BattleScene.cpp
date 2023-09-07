#include "BattleScene.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

#include "../BattleManager/ExistUnits.h"

#include "../OperationConfig.h"

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/Enemy_Act/EnemyActList.h"

void BattleScene::OnInitialize()
{
	// ---- 背景
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_SukasiTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "Sukasi.png");
	m_BackTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "battle_scene_frame.png");



	Pl = std::make_shared<Player>();
	Pl->OnInitialize();
	Pl->StartTurn();

	/*En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_1);
	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_2);
	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_3);
	Mgr.OnInitialize(Pl, En);
	ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());*/

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
	ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());

	/*En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_Boss_1);
	Mgr.OnInitialize(Pl, En);
	ExistUnits::Instance()->Set(Pl.get(), En[0].get());*/

	stage.reset(new PanelManager());
	stage->Initialize();
	block.reset(new BlockManager());
	//中央指定
	block->SetCenter(stage->GetCenter());
	block->Initialize();


	ExistUnits::Instance()->SetMapChipPtr(stage->GetMapChipPtr());
	ExistUnits::Instance()->SetStageManagerPtr(stage.get());
}

void BattleScene::OnUpdate()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	// ウェーブ終了・次ウェーブスタート
	if (Mgr.ChangeNextWave()) {
		NextWave();
	}

	//セット可能ならセットする
	if (ExistUnits::Instance()->m_NowTurn == 0) {
		PlayerTurn();
	}

	//リセット
	if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_I)) {
		stage->Reset();
	}

	Mgr.OnUpdate();

	stage->Update();
	block->Update();

	//ゲームオーバーもしくはクリアをしたらシーンを切り替えられるようにする
	if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_RETURN)) {
		KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title");
	}
}

void BattleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});

	// 背景色
	DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), Color(50, 49, 59, 255));
	// Vキーを押してる間だけ透かしを描画
	if (UsersInput::Instance()->KeyInput(DIK_V)) {
		DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_SukasiTex);
	}
	// 背景描画
	DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_BackTex);

	// プレイヤーはバトル中以外でも描画する為こっち
	Pl->OnDraw();

	stage->Draw();
	block->Draw();

	Mgr.OnDraw();

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
}

void BattleScene::OnFinalize()
{
	En.clear();
	PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
	EnemyActions::EnemyActionMgr::Instance()->AllClear();
}

void BattleScene::SetStage(std::string StageName)
{
	m_NowStage = Stages::StageList::Instance()->GetStage(StageName);
}

void BattleScene::NextWave()
{
	// 次ウェーブに変更
	m_NowWave++;
	// 次ウェーブの敵を取得
	En.clear();
	std::vector<EnemysData::EnemyData> EnemyData = m_NowStage.GetWaveEnemyIndex(m_NowWave);
	for (auto& data : EnemyData) {
		En.emplace_back(std::make_shared<Enemy>());
		GetUnitPtr<Enemy>(En.back())->SetEnemyData(data);
	}
	Mgr.SetUnits(Pl, En);
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

BattleScene::BattleScene()
{
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
		if (!stage->JudgeSet(nowMapchip, shape, attribute, color)) { return; }
		//次の使用ブロックをセットする
		block->ChangeBlock();
	}
}
