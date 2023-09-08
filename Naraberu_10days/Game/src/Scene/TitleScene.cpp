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

	selectNum = Select::title;
	stageNum = 0;
	onSelect = false;

	if (ExistUnits::Instance()->m_ChangeStageSelect == true) {
		selectNum = Select::stageSelect;
		onSelect = true;
		title->SetIsStageSelectInMove();
		ExistUnits::Instance()->m_ChangeStageSelect = false;
	}
}

void TitleScene::OnUpdate()
{
	//タイトルかステージセレクトかの判定
	if (selectNum == Select::stageSelect && onSelect) {
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
	if (!onSelect) {
		if (selectNum == Select::title) {
			KuroEngine::DrawFunc2D::DrawExtendGraph2D({ 100,100 }, { 500,500 }, stageSelectBarTex);
		} else if (selectNum == Select::stageSelect) {
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
		selectNum = Select(int(selectNum) + 1);
		if (selectNum >= Select::size) {
			selectNum = Select(int(Select::size) - 1);
		}
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		selectNum = Select(int(selectNum) - 1);
		if (selectNum < Select::title) {
			selectNum = Select::title;
		}
	}

	//ステージセレクトへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE,
		OperationConfig::INPUT_PATTERN::ON_TRIGGER) && selectNum == Select::stageSelect) {
		onSelect = true;
		title->SetIsStageSelectInMove();
	}
}

void TitleScene::StageSelect()
{
	//battleへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		m_StageName = "Stage" + std::to_string(title->GetStageNum());
		KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle");
	}

	//タイトルへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::CANCEL, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		onSelect = false;
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