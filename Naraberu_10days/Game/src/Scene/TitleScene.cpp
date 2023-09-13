#include "TitleScene.h"
#include "../OperationConfig.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "../BattleManager/ExistUnits.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"../SoundConfig.h"
#include"FrameWork/AudioApp.h"
#include"ForUser/Debugger.h"

void TitleScene::OnInitialize()
{
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

	character->SetMoveTitle(false);

	if (ExistUnits::Instance()->m_ChangeStageSelect == true) {
		selectNum = Select::stageSelect;
		onSelect = true;
		title->SetIsStageSelectInMove();
		ExistUnits::Instance()->m_ChangeStageSelect = false;
		character->SetMoveStageSelect(true);
	}

	SoundConfig::Instance()->SwitchBGM(SoundConfig::BGM_SUB);

	KuroEngine::Debugger::Register(
		{
			SoundConfig::Instance(),
		});
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

	//遷移移動量
	float move = character->GetMove();

	//文字
	const Vec2<float> selectPos = { 65.0f + move,50.0f };
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

	const Vec2<float> titelPos = { 30.0f + move,421.0f };
	DrawFunc2D::DrawGraph(titelPos, titleTex);

}

void TitleScene::OnImguiDebug()
{
	KuroEngine::Debugger::Draw();
	//ImGui::Begin("title");
	//ImGui::End();
}

void TitleScene::OnFinalize()
{
	KuroEngine::Debugger::ClearRegister();
}

TitleScene::TitleScene()
{
	title.reset(new TitleUi());
	title->Initialize();

	character.reset(new TitleVtuber());
	character->Initialize();
}

void TitleScene::Title()
{
	//移動
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN)) {
		selectNum = Select(int(selectNum) + 1);
		if (selectNum >= Select::size) {
			selectNum = Select(int(Select::size) - 1);
			SoundConfig::Instance()->Play(SoundConfig::SE_CANNOT_SELECT);
		}
		else SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		selectNum = Select(int(selectNum) - 1);
		if (selectNum < Select::stageSelect) {
			selectNum = Select::stageSelect;
			SoundConfig::Instance()->Play(SoundConfig::SE_CANNOT_SELECT);
		}
		else SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}

	//ステージセレクトへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::DONE,
		OperationConfig::INPUT_PATTERN::ON_TRIGGER) && selectNum == Select::stageSelect) {
		if (!title->GetIsStageSelectOutMove() && !character->GetisMove()) {
			onSelect = true;
			title->SetIsStageSelectInMove();
			SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
			character->SetMoveStageSelect();
		}
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
		if (!title->GetIsStageMove()) {
			ExistUnits::Instance()->m_StageName = "Stage" + std::to_string(title->GetStageNum() - 1);
			// 仮で5を選ぶとチュートリアルになるように
			if (title->GetStageNum() == 1) {
				ExistUnits::Instance()->m_StageName = "Tutorial";
			}
			else if (title->GetStageNum() == 6) {
				ExistUnits::Instance()->m_StageName = "Challenge";
			}
			ExistUnits::Instance()->m_Now_Stage_Index = title->GetStageNum() - 1;
			KuroEngine::KuroEngineDevice::Instance()->ChangeScene("Battle", &m_Fade);
			SoundConfig::Instance()->Play(SoundConfig::SE_DONE);
		}
	}

	//タイトルへ
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::OPERATION_TYPE::CANCEL, OperationConfig::INPUT_PATTERN::ON_TRIGGER)) {
		if (!title->GetIsStageSelectInMove() && !character->GetisMove()) {
			onSelect = false;
			title->SetIsStageSelectOutMove();
			SoundConfig::Instance()->Play(SoundConfig::SE_CANCEL);
			character->SetMoveTitle(false);
		}
	}

	if (title->GetIsStageSelectInMove()) { return; }
	//移動
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_DOWN)) {
		if (!title->GetIsStageMove()) {
			if(title->SetStageSelectMove(true))SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
			else SoundConfig::Instance()->Play(SoundConfig::SE_CANNOT_SELECT);
		}
	}
	if (OperationConfig::Instance()->GetSelectVec(OperationConfig::SELECT_VEC::SELECT_VEC_UP)) {
		if (!title->GetIsStageMove()) {
			if (title->SetStageSelectMove(false))SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
			else SoundConfig::Instance()->Play(SoundConfig::SE_CANNOT_SELECT);

		}
	}
}