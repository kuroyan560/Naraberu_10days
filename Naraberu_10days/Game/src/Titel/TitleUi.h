#pragma once
#include "KuroEngineDevice.h"

#include <array>

class TitleUi
{
private:

	enum class POS {
		up,
		center,
		down,
		non,
	};

	struct StageTexInfo {
		KuroEngine::Vec2<float> pos;
		float scale;
		POS nowPos;
		POS nextPos;
	};

public:
	TitleUi();

	void Initialize();

	void Update();

	void Draw();

	/// <summary>
	/// �X�e�[�W�Z���N�g�Ɉړ�����Ƃ��̏���
	/// </summary>
	void StageSelectInMove();

	/// <summary>
	/// �^�C�g���Ɉړ�����Ƃ��̏���
	/// </summary>
	void StageSelectOutMove();

	/// <summary>
	/// �X�e�[�W�Z���N�g�ړ�
	/// </summary>
	/// <param name="_number"></param>
	void StageSelectMove(int _number);
	
	/// <summary>
	/// �X�e�[�W�Z���N�g�ړ�����
	/// </summary>
	/// <param name="_upDown">false->�Ⴂ�ԍ��ɍs��(down) true->�����ԍ��ɍs��(up)</param>
	/// <returns>�I�����ω�������</returns>
	bool SetStageSelectMove(int _upDown);

	void SetIsStageSelectInMove(bool _isStageSelectInMove = true) { isStageSelectInMove = _isStageSelectInMove; }
	void SetIsStageSelectOutMove(bool _isStageSelectOutMove = true) { isStageSelectOutMove = _isStageSelectOutMove; }
	bool GetIsStageMove() { return isStageMove; }
	int GetStageNum() { return nowStageNum + 1; }
	bool GetIsStageSelectInMove() { return isStageSelectInMove; }
	bool GetIsStageSelectOutMove() { return isStageSelectOutMove; }

	// �w�i
	std::shared_ptr<KuroEngine::TextureBuffer> backTex;
	//�^�C�g��
	std::shared_ptr<KuroEngine::TextureBuffer> titleTex;
	//�I����
	std::shared_ptr<KuroEngine::TextureBuffer> stageTex;
	//�X�e�[�W�I���o�[
	std::shared_ptr<KuroEngine::TextureBuffer> stageSelectBarTex;
	//����
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11> numTex;
	//����
	std::shared_ptr<KuroEngine::TextureBuffer> tutorialTex;
	//�N���A
	std::shared_ptr<KuroEngine::TextureBuffer> challengeTex;
	//�N���A
	std::shared_ptr<KuroEngine::TextureBuffer> clearTex;

	//�^�C�g������X�e�[�W�Z���N�g�ֈړ�����
	bool isStageSelectInMove;
	float stageSelectInMoveTimer;
	//�X�e�[�W�Z���N�g����^�C�g���ֈړ�����
	bool isStageSelectOutMove;
	float stageSelectOutMoveTimer;
	//�X�e�[�W�Z���N�g�̈ړ����s��
	bool isStageMove;
	//�Z���N�g�ړ�
	float selectTimer;
	//���݂̃X�e�[�W�ԍ�
	int nowStageNum;
	//�X�e�[�W�ω�+-����
	bool stageUpDown;
	//�X�e�[�W��
	static const int stage = 8;
	//�Z���N�g���
	std::array<StageTexInfo, stage> stageTexInfo;
};

