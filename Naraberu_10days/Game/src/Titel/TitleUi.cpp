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

	// �w�i
	backTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "select_scene_sukasi.png");
	//�^�C�g��
	titleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "title.png");
	//�ustage�v����
	stageTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "stage.png");
	//�X�e�[�W�I���o�[
	stageSelectBarTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "stage_name_plate.png");
	//����
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(numTex.data(),
		TexDir + "select_stage_number.png", int(numTex.size()), { int(numTex.size()),1 });
	//�`���[�g���A������
	tutorialTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "tutorial.png");

	//�^�C�g������X�e�[�W�Z���N�g�ֈړ�����
	isStageSelectInMove = false;
	//�X�e�[�W�Z���N�g�̈ړ����s��
	isStageMove = false;
	//���݂̃X�e�[�W�ԍ�
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
	//�X�e�[�W�Z���N�g�ł̃X�e�[�W�ړ�
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

	//�X�e�[�W�Z���N�g��in out
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
		//���n
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ stageTexInfo[i].pos.x,stageTexInfo[i].pos.y },
			{ stageTexInfo[i].pos.x + sitaji_size.x * stageTexInfo[i].scale,stageTexInfo[i].pos.y + sitaji_size.y * stageTexInfo[i].scale }, stageSelectBarTex);
		//����
		if (i > 0) {
			//�ustage�v
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
			//�X�e�[�W�ԍ�
			KuroEngine::DrawFunc2D::DrawNumber2D(i, inpos, numTex.data());
		} else {
			KuroEngine::Vec2<float> inpos = {
			stageTexInfo[i].pos.x + tutorial_moji_dist.x * stageTexInfo[i].scale + 100.0f * stageTexInfo[i].scale,
			stageTexInfo[i].pos.y + tutorial_moji_dist.x * stageTexInfo[i].scale };
			//�`���[�g���A������
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

	//��Ɉړ�
	//�ォ���O
	if (stageTexInfo[_number].nowPos == POS::up && stageTexInfo[_number].nextPos == POS::non) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, u_pos.x, 1500.0f);
	}
	//���������
	else if (stageTexInfo[_number].nowPos == POS::center && stageTexInfo[_number].nextPos == POS::up) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, c_pos, u_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, 1.0f, u_scale);
	}
	//�����璆��
	else if (stageTexInfo[_number].nowPos == POS::down && stageTexInfo[_number].nextPos == POS::center) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, d_pos, c_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, d_scale, 1.0f);
	}
	//��O���牺
	else if (stageTexInfo[_number].nowPos == POS::non && stageTexInfo[_number].nextPos == POS::down) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, 1500.0f, d_pos.x);
		stageTexInfo[_number].pos.y = d_pos.y;
	}
	//���Ɉړ�
	//��O�����
	else if (stageTexInfo[_number].nowPos == POS::non && stageTexInfo[_number].nextPos == POS::up) {
		stageTexInfo[_number].pos.x = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer,1500.0f, u_pos.x);
		stageTexInfo[_number].pos.y = u_pos.y;
	}
	//�ォ�璆��
	else if (stageTexInfo[_number].nowPos == POS::up && stageTexInfo[_number].nextPos == POS::center) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, u_pos, c_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, u_scale, 1.0f);
	}
	//�������牺
	else if (stageTexInfo[_number].nowPos == POS::center && stageTexInfo[_number].nextPos == POS::down) {
		stageTexInfo[_number].pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, c_pos, d_pos);
		stageTexInfo[_number].scale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			selectTimer, maxTimer, 1.0f, d_scale);
	}
	//�������O
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
	//�����ԍ��ɍs��(up)
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
	//�Ⴂ�ԍ��ɍs��(down)
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
