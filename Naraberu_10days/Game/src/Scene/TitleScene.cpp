#include "TitleScene.h"
#include "../OperationConfig.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "../BattleManager/ExistUnits.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

void TitleScene::OnInitialize()
{
	title.reset(new TitleUi());
	title->Initialize();

	std::string TexDir = "resource/user/tex/select_scene/";
	stageSelectBarTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "stage_name_plate.png");

	ExistUnits::Instance()->selectNum = ExistUnits::Select::title;
	stageNum = 0;
	ExistUnits::Instance()->onSelect = false;
}

void TitleScene::OnUpdate()
{
	//タイトルかステージセレクトかの判定
	if (ExistUnits::Instance()->selectNum == ExistUnits::Select::stageSelect && ExistUnits::Instance()->onSelect) {
		StageSelect();
	} else {
		Title();
	}
	
	title->Update();
}

void TitleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});

	title->Draw();

	//debug用
	//後で画像差し替え
	if (!ExistUnits::Instance()->onSelect) {
		if (ExistUnits::Instance()->selectNum == ExistUnits::Select::title) {
			KuroEngine::DrawFunc2D::DrawExtendGraph2D({ 100,100 }, { 500,500 }, stageSelectBarTex);
		} else if (ExistUnits::Instance()->selectNum == ExistUnits::Select::stageSelect) {
			KuroEngine::DrawFunc2D::DrawExtendGraph2D({ 200,200 }, { 500,500 }, stageSelectBarTex);
		}
	}
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

void TitleScene::Title()
{
	//移動
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN)) {
		ExistUnits::Instance()->selectNum = ExistUnits::Select(int(ExistUnits::Instance()->selectNum) + 1);
		if (ExistUnits::Instance()->selectNum >= ExistUnits::Select::size) {
			ExistUnits::Instance()->selectNum = ExistUnits::Select(int(ExistUnits::Select::size) - 1);
		}
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		ExistUnits::Instance()->selectNum = ExistUnits::Select(int(ExistUnits::Instance()->selectNum) - 1);
		if (ExistUnits::Instance()->selectNum < ExistUnits::Select::title) {
			ExistUnits::Instance()->selectNum = ExistUnits::Select::title;
		}
	}

	//ステージセレクトへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE,
		OperationConfig::INPUT_PATTERN::ON_TRIGGER) && ExistUnits::Instance()->selectNum == ExistUnits::Select::stageSelect) {
		ExistUnits::Instance()->onSelect = true;
		title->SetIsStageSelectInMove();
	}
}

void TitleScene::StageSelect()
{
	//battleへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		ExistUnits::Instance()->m_StageName = "Stage" + std::to_string(title->GetStageNum());
		KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle");
	}

	//タイトルへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::CANCEL, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		ExistUnits::Instance()->onSelect = false;
		title->SetIsStageSelectOutMove();
	}

	//移動
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN)) {
		if (!title->GetIsStageMove()) {
			title->SetStageSelectMove(true);
		}
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		if (!title->GetIsStageMove()) {
			title->SetStageSelectMove(false);
		}
	}
}