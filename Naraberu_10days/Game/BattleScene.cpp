#include "BattleScene.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"


void BattleScene::OnInitialize()
{
	// ---- ”wŒi
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_SukasiTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "Sukasi.png");
	m_BackTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "battle_scene_frame.png");



	Pl = std::make_shared<Player>();
	Pl->OnInitialize();
	Pl->StartTurn();


	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_1);
	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_2);

	Mgr.OnInitialize(Pl, En);
}

void BattleScene::OnUpdate()
{
	Mgr.OnUpdate();
}

void BattleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});

	// ”wŒiF
	DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), Color(50, 49, 59, 255));
	// VƒL[‚ð‰Ÿ‚µ‚Ä‚éŠÔ‚¾‚¯“§‚©‚µ‚ð•`‰æ
	if (UsersInput::Instance()->KeyInput(DIK_V)) {
		DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_SukasiTex);
	}
	// ”wŒi•`‰æ
	DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_BackTex);

	// ƒvƒŒƒCƒ„[‚Íƒoƒgƒ‹’†ˆÈŠO‚Å‚à•`‰æ‚·‚éˆ×‚±‚Á‚¿
	Pl->OnDraw();

	Mgr.OnDraw();
}

void BattleScene::OnImguiDebug()
{
	Mgr.OnImguiDebug();
}

void BattleScene::OnFinalize()
{
}

BattleScene::BattleScene()
{
}
