#include "GameScene.h"
#include"FrameWork/ImguiApp.h"


GameScene::GameScene()
{
}

void GameScene::OnInitialize()
{
}

void GameScene::OnUpdate()
{
}

#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
void GameScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
		D3D12App::Instance()->GetBackBuffRenderTarget()
		});
}

void GameScene::OnImguiDebug()
{
	ImGui::Begin("GameScene");
	ImGui::Text("Naw scene is GameScene.");
	ImGui::End();
}

void GameScene::OnFinalize()
{
}

