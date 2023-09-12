#include "TitleUi.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../RefreshRate.h"

const KuroEngine::Vec2<float> c_pos = { 700.0f,400.0f };
const KuroEngine::Vec2<float> u_pos = { 950.0f,250.0f };
const KuroEngine::Vec2<float> d_pos = { 850.0f,600.0f };

void TitleUi::Initialize()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/select_scene/";

	// 背景
	backTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "select_scene_sukasi.png");
	//タイトル
	titleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "title.png");
	//「stage」文字
	stageTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "stage.png");
	//ステージ選択バー
	stageSelectBarTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "stage_name_plate.png");
	//数字
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(numTex.data(),
		TexDir + "select_stage_number.png", int(numTex.size()), { int(numTex.size()),1 });
	//チュートリアル文字
	tutorialTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "tutorial.png");

	//タイトルからステージセレクトへ移動する
	isStageSelectInMove = false;
	//ステージセレクトの移動を行う
	isStageMove = false;
	//現在のステージ番号
	nowStageNum = 0;


	const float u_scale = 0.7f;
	const float d_scale = 0.8f;

	stageTexInfo[0].pos = { 1500.0f,c_pos.y };
	stageTexInfo[0].nowPos = POS::center;
	stageTexInfo[0].nextPos = POS::center;
	stageTexInfo[0].scale = 1.0f;

	stageTexInfo[1].pos = { 1500.0f,d_pos.y };
	stageTexInfo[1].nowPos = POS::down;
	stageTexInfo[1].nextPos = POS::down;
	stageTexInfo[1].scale = d_scale;

	for (int i = 2; i < stage; i++) {
		stageTexInfo[i].pos = { 1500.f,d_pos.y };
		stageTexInfo[i].nowPos = POS::non;
		stageTexInfo[i].nextPos = POS::non;
		stageTexInfo[i].scale = d_scale;
	}

	selectTimer = 0;
}

void TitleUi::Update()
{
	//ステージセレクトでのステージ移動
	if (isStageMove) {
		for (int i = 0; i< int(stageTexInfo.size()); i++) {
			StageSelectMove(i);
		}
		selectTimer++;
		if (selectTimer > 20 * RefreshRate::RefreshRate_Mag) {
			isStageMove = false;
			selectTimer = 0;
			if (stageUpDown) {
				nowStageNum++;
			} else {
				nowStageNum--;
			}
		}
	}

	//ステージセレクトのin out
	if (isStageSelectInMove) {
		StageSelectInMove();
	} else if (isStageSelectOutMove) {
		StageSelectOutMove();
	}
}

void TitleUi::Draw()
{
	const KuroEngine::Vec2<float> sitaji_size = { 638.0f,160.0f };
	const KuroEngine::Vec2<float> stage_moji_size = { 257.0f,73.0f };
	const KuroEngine::Vec2<float> stage_moji_dist = { 90.0f,50.0f };
	const KuroEngine::Vec2<float> number_dist = { 390.0f,50.0f };
	const KuroEngine::Vec2<float> tutorial_moji_size = { 361.0f,57.0f };
	const KuroEngine::Vec2<float> tutorial_moji_dist= { 50.0f,100.0f };

	for (int i=0;i< stage;i++) {
		//下地
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ stageTexInfo[i].pos.x,stageTexInfo[i].pos.y },
			{ stageTexInfo[i].pos.x + sitaji_size.x * stageTexInfo[i].scale,stageTexInfo[i].pos.y + sitaji_size.y * stageTexInfo[i].scale }, stageSelectBarTex);
		//数字
		if (i > 0) {
			//「stage」
			KuroEngine::DrawFunc2D::DrawExtendGraph2D({ stageTexInfo[i].pos.x + stage_moji_dist.x,stageTexInfo[i].pos.y + stage_moji_dist.y },
				{ stageTexInfo[i].pos.x + stage_moji_dist.x * stageTexInfo[i].scale + stage_moji_size.x * stageTexInfo[i].scale,
				stageTexInfo[i].pos.y + stage_moji_dist.y * stageTexInfo[i].scale + stage_moji_size.y * stageTexInfo[i].scale }, stageTex);

			KuroEngine::Vec2<float> inpos = {
			stageTexInfo[i].pos.x + number_dist.x * stageTexInfo[i].scale,
			stageTexInfo[i].pos.y + number_dist.y * stageTexInfo[i].scale};
			//1-
			KuroEngine::DrawFunc2D::DrawNumber2D(1, inpos, numTex.data(), { 1.0f,1.0f },
				1.0f, 0.0f, KuroEngine::HORIZONTAL_ALIGN::LEFT, KuroEngine::VERTICAL_ALIGN::TOP, 0, -1, 10);

			inpos = {
			stageTexInfo[i].pos.x + number_dist.x * stageTexInfo[i].scale + 120.0f * stageTexInfo[i].scale,
			stageTexInfo[i].pos.y + number_dist.y * stageTexInfo[i].scale};
			//ステージ番号
			KuroEngine::DrawFunc2D::DrawNumber2D(i, inpos, numTex.data());
		} else {
			KuroEngine::Vec2<float> inpos = {
			stageTexInfo[i].pos.x + tutorial_moji_dist.x * stageTexInfo[i].scale + 100.0f * stageTexInfo[i].scale,
			stageTexInfo[i].pos.y + tutorial_moji_dist.x * stageTexInfo[i].scale };
			//チュートリアル文字
			KuroEngine::DrawFunc2D::DrawExtendGraph2D(inpos,
				{ inpos.x + tutorial_moji_size.x * stageTexInfo[i].scale,inpos.y + tutorial_moji_size.y * stageTexInfo[i].scale }, tutorialTex);
		}
	}
}

void TitleUi::StageSelectInMove()
{
	const float maxTimer = 50.0f * RefreshRate::RefreshRate_Mag;

	stageSelectInMoveTimer++;

	stageTexInfo[nowStageNum].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
		stageSelectInMoveTimer, maxTimer,1500.0f, c_pos.x);
	if (nowStageNum - 1 >= 0) {
		stageTexInfo[nowStageNum - 1].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			stageSelectInMoveTimer, maxTimer, 1500.0f, u_pos.x);
	}
	if (nowStageNum + 1 < stage) {
		stageTexInfo[nowStageNum + 1].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			stageSelectInMoveTimer, maxTimer, 1500.0f, d_pos.x);
	}

	if (stageSelectInMoveTimer >= maxTimer) {
		isStageSelectInMove = false;
		stageSelectInMoveTimer = 0;
	}
}

void TitleUi::StageSelectOutMove()
{
	const float maxTimer = 50.0f * RefreshRate::RefreshRate_Mag;

	stageSelectOutMoveTimer++;

	stageTexInfo[nowStageNum].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Back,
		stageSelectOutMoveTimer, maxTimer, c_pos.x, 1500.0f);
	if (nowStageNum - 1 >= 0) {
		stageTexInfo[nowStageNum - 1].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Back,
			stageSelectOutMoveTimer, maxTimer, u_pos.x, 1500.0f);
	}
	if (nowStageNum + 1 < stage) {
		stageTexInfo[nowStageNum + 1].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Back,
			stageSelectOutMoveTimer, maxTimer, d_pos.x, 1500.0f);
	}

	if (stageSelectOutMoveTimer >= maxTimer) {
		isStageSelectOutMove = false;
		stageSelectOutMoveTimer = 0;
	}
}

void TitleUi::StageSelectMove(int _number)
{
	if (stageTexInfo[_number].nowPos == stageTexInfo[_number].nextPos) { return; }

	const float maxTimer = 20.0f * RefreshRate::RefreshRate_Mag;

	const float u_scale = 0.7f;
	const float d_scale = 0.8f;

	//上に移動
	//上から場外
	if (stageTexInfo[_number].nowPos == POS::up && stageTexInfo[_number].nextPos == POS::non) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, u_pos.x, 1500.0f);
	}
	//中央から上
	else if (stageTexInfo[_number].nowPos == POS::center && stageTexInfo[_number].nextPos == POS::up) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, c_pos, u_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, 1.0f, u_scale);
	}
	//下から中央
	else if (stageTexInfo[_number].nowPos == POS::down && stageTexInfo[_number].nextPos == POS::center) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, d_pos, c_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, d_scale, 1.0f);
	}
	//場外から下
	else if (stageTexInfo[_number].nowPos == POS::non && stageTexInfo[_number].nextPos == POS::down) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, 1500.0f, d_pos.x);
		stageTexInfo[_number].pos.y = d_pos.y;
	}
	//下に移動
	//場外から上
	else if (stageTexInfo[_number].nowPos == POS::non && stageTexInfo[_number].nextPos == POS::up) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer,1500.0f, u_pos.x);
		stageTexInfo[_number].pos.y = u_pos.y;
	}
	//上から中央
	else if (stageTexInfo[_number].nowPos == POS::up && stageTexInfo[_number].nextPos == POS::center) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, u_pos, c_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, u_scale, 1.0f);
	}
	//中央から下
	else if (stageTexInfo[_number].nowPos == POS::center && stageTexInfo[_number].nextPos == POS::down) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, c_pos, d_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, 1.0f, d_scale);
	}
	//下から場外
	else if (stageTexInfo[_number].nowPos == POS::down && stageTexInfo[_number].nextPos == POS::non) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, d_pos.x, 1500.0f);
	}

	if (selectTimer >= maxTimer) {
		stageTexInfo[_number].nowPos= stageTexInfo[_number].nextPos;
	}
}

bool TitleUi::SetStageSelectMove(int _upDown)
{
	stageUpDown = _upDown;
	//高い番号に行く(up)
	if (_upDown) {
		if (nowStageNum == stage - 1) { return false; }
		if (nowStageNum - 1 >= 0) {
			stageTexInfo[nowStageNum - 1].nextPos = POS::non;
		}
		stageTexInfo[nowStageNum].nextPos = POS::up;
		if (nowStageNum + 1 < stage) {
			stageTexInfo[nowStageNum + 1].nextPos = POS::center;
		}
		if (nowStageNum + 2 < stage) {
			stageTexInfo[nowStageNum + 2].nextPos = POS::down;
		}
	}
	//低い番号に行く(down)
	else {
		if (nowStageNum == 0) { return false; }
		if (nowStageNum - 2 >= 0) {
			stageTexInfo[nowStageNum - 2].nextPos = POS::up;
		}
		if (nowStageNum - 1 >= 0) {
			stageTexInfo[nowStageNum - 1].nextPos = POS::center;
		}
		stageTexInfo[nowStageNum].nextPos = POS::down;
		if (nowStageNum + 1 < stage) {
			stageTexInfo[nowStageNum + 1].nextPos = POS::non;
		}
	}
	isStageMove = true;
	selectTimer = 0;
	return true;
}
