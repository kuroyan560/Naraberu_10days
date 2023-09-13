#pragma once
#include "KuroEngineDevice.h"
#include "../Titel/TitleUi.h"
#include "../Fade/Fade.h"
#include "../Titel/TitleVtuber.h"
#include<memory>
class ParticleEmitter;
#include"ForUser/Timer.h"

class TitleScene : public KuroEngine::BaseScene
{	
	//UI
	std::shared_ptr<TitleUi> title;

	//�L�����N�^�[�A�j���[�V����
	std::shared_ptr<TitleVtuber> character;

	//�^�C�g��
	std::shared_ptr<KuroEngine::TextureBuffer> titleTex;
	//start����
	std::shared_ptr<KuroEngine::TextureBuffer> startTex;
	//quit����
	std::shared_ptr<KuroEngine::TextureBuffer> quitTex;
	//title�I���o�[
	std::shared_ptr<KuroEngine::TextureBuffer> titleBarTex;
	//�X�e�[�W�I���o�[
	std::shared_ptr<KuroEngine::TextureBuffer> stageSelectBarTex;
	//�������i��ŏ����j
	std::shared_ptr<KuroEngine::TextureBuffer> m_SukasiTex;

	//�X�e�[�W�ԍ�
	int stageNum;

	// �t�F�[�h
	Fade m_Fade;

	//�^�C�g���V�[���p
	enum class Select {
		stageSelect,
		exit,
		size,
	};

	// �I����
	Select selectNum;
	// �I�����s���Ă���
	bool onSelect;

	KuroEngine::Timer m_backPtEmitTimer;
	std::weak_ptr<ParticleEmitter>m_ptEmitter;

	/// <summary>
	/// �^�C�g��
	/// </summary>
	void Title();

	/// <summary>
	/// �X�e�[�W�Z���N�g
	/// </summary>
	void StageSelect();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	TitleScene();

};

