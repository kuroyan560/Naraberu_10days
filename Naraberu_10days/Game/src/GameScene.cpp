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

void GameScene::OnDraw()
{
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

