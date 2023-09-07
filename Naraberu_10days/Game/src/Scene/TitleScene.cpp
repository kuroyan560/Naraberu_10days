#include "TitleScene.h"
#include "../OperationConfig.h"

void TitleScene::OnInitialize()
{
}

void TitleScene::OnUpdate()
{
	if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_RETURN)) {
		KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle");
	}
}

void TitleScene::OnDraw()
{
}

void TitleScene::OnImguiDebug()
{
	ImGui::Begin("title");
	ImGui::End();
}

void TitleScene::OnFinalize()
{
}

TitleScene::TitleScene()
{
}
