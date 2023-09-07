#include "TitleScene.h"
#include "../OperationConfig.h"
#include "../../src/engine/DirectX12/D3D12App.h"

void TitleScene::OnInitialize()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/select_scene/";

	// �w�i
	backTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "select_scene_sukasi.png");

	//�^�C�g��
	titleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "select_scene_sukasi.png");
	//�I����
	stageTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "stage.png");
	//����
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(numTex.data(), TexDir + "pass_number.png", 3, { 3, 1 });

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
