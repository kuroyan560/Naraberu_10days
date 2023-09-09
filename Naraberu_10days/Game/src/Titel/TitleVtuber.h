#pragma once
#include "KuroEngineDevice.h"
#include <array>
#include"ForUser/HandShake.h"

class TitleVtuber
{
	static const int TrajectorySize=10;
	struct ObjectInfo {
		KuroEngine::Vec2<float> pos;
		float rota;
		float timer;
		int number;
	};

	struct Trajectory {
		//�O�Օ`��
		bool isTrajectoryDraw;
		//���W
		KuroEngine::Vec2<float> pos;
		//���\
		bool back;
	};

	struct BigPrismInfo {
		//��ԉ�
		int easeNum;
		//�ǂ���̕�Ԃ��g�p���邩
		bool useEase;
		//���\
		bool back;
		//�O��
		std::array<Trajectory, TrajectorySize> trajectory;
	};

public:
	TitleVtuber();

	void Initialize();

	void Update();

	void Draw();

	//�v���Y�����A�j���[�V����
	void SmallPrismAnimation();

	//�v���Y����A�j���[�V����
	void BigPrismAnimation();

	void MoveStageSelect();

	//�X�e�[�W�Z���N�g�I���ŉ��ɃY����
	void SetMoveStageSelect(bool _mouzureteru = false) {
		if (_mouzureteru) {
			move = -1500.0f;
			LR = false;
			isRightMove = false;
		} else {
			isLeftMove = true;
			moveTimer = 0;
		}
	}

	//�^�C�g���ɖ߂鎞
	void SetMoveTitle(bool _mouzureteru = false) {
		if (_mouzureteru) {
			move = 0.0f;
			LR = true;
		} else {
			isRightMove = true;
			moveTimer = 0;
		}
	}

	float GetMove() { return move; }

private:

	//�L����
	std::shared_ptr<KuroEngine::TextureBuffer> characterTex;
	//�������v���Y��
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,3> smallPrismTex;
	//�傫���v���Y��
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 3> bigPrismTex;

	//�������v���Y��
	std::array<ObjectInfo,3> smallPrism;
	//�傫���v���Y��
	std::array<ObjectInfo, 3> bigPrism;
	//�ǂ���̕�Ԃ��g�p���邩
	std::array<BigPrismInfo,3> bigPrismInfo;

	//�L�����̗h��
	KuroEngine::HandShake characterShake;

	//�X�e�[�W�Z���N�g�I���ŉ��ɃY����
	bool isLeftMove;
	//�^�C�g���ɖ߂�Ɖ��ɂ����
	bool isRightMove;
	//�ړ���
	float move;
	//�^�C�}�[
	float moveTimer;
	//�ǂ���ɂ��邩R true L false
	bool LR;
};

