#include "TitleScene.h"
#include "../OperationConfig.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "../BattleManager/ExistUnits.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"../SoundConfig.h"

void TitleScene::OnInitialize()
{
	title.reset(new TitleUi());
	title->Initialize();

	character.reset(new TitleVtuber());
	character->Initialize();

	std::string TexDir_select = "resource/user/tex/select_scene/";
	stageSelectBarTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir_select + "stage_name_plate.png");

	std::string TexDir_title = "resource/user/tex/title_scene/";
	titleTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir_title + "title.png");
	startTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir_title + "start.png");
	quitTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir_title + "quit.png");
	titleBarTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir_title + "select_line.png");
	m_SukasiTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir_title + "sukasi.png");

	selectNum = Select::stageSelect;
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
	character->Update();
}

void TitleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});

	// Vキーを押してる間だけ透かしを描画
	if (UsersInput::Instance()->KeyInput(DIK_V)) {
		DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_SukasiTex);
	}

	const Vec2<float> selectPos = { 65.0f,50.0f };
	DrawFunc2D::DrawGraph(selectPos, startTex);
	DrawFunc2D::DrawGraph({ selectPos.x,selectPos.y + 115.0f }, quitTex);

	//タイトルセレクト
	if (!onSelect) {
		if (selectNum == Select::stageSelect) {
			DrawFunc2D::DrawGraph({ 0.0f,selectPos.y - 45.0f }, titleBarTex);
		} else if (selectNum == Select::exit) {
			DrawFunc2D::DrawGraph({ 0.0f,selectPos.y - 45.0f + 115.0f }, titleBarTex);
		}
	}

	title->Draw();
	character->Draw();

	const Vec2<float> titelPos = { 30.0f,421.0f };
	DrawFunc2D::DrawGraph(titelPos, titleTex);

}

void TitleScene::OnImguiDebug()
{
	//ImGui::Begin("title");
	//ImGui::End();
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
		SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		selectNum = Select(int(selectNum) - 1);
		if (selectNum < Select::stageSelect) {
			selectNum = Select::stageSelect;
		}
		SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}

	//ステージセレクトへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE,
		OperationConfig::INPUT_PATTERN::ON_TRIGGER) && selectNum == Select::stageSelect) {
		onSelect = true;
		title->SetIsStageSelectInMove();
		SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
	}

	//ゲームを落とす処理
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE,
		OperationConfig::INPUT_PATTERN::ON_TRIGGER) && selectNum == Select::exit) {
		KuroEngine::KuroEngineDevice::Instance()->GameEnd();
		SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
	}
}

void TitleScene::StageSelect()
{
	//battleへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		ExistUnits::Instance()->m_StageName = "Stage" + std::to_string(title->GetStageNum());
		KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle", &m_Fade);
		SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
	}

	//タイトルへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::CANCEL, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		onSelect = false;
		title->SetIsStageSelectOutMove();
		SoundConfig::Instance()->Play(SoundConfig::SE_CANCEL);
	}

	//移動
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN)) {
		if (!title->GetIsStageMove()) {
			title->SetStageSelectMove(true);
		}
		SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		if (!title->GetIsStageMove()) {
			title->SetStageSelectMove(false);
		}
		SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}
}