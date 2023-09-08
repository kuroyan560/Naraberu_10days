#pragma once
#include "KuroEngineDevice.h"
#include "../TitleUi.h"
#include "../Fade/Fade.h"

class TitleScene : public KuroEngine::BaseScene
{	
	//UI
	std::shared_ptr<TitleUi> title;

	//�X�e�[�W�I���o�[
	std::shared_ptr<KuroEngine::TextureBuffer> stageSelectBarTex;

	//�X�e�[�W�ԍ�
	int stageNum;

	// �t�F�[�h
	Fade m_Fade;

	//�^�C�g���V�[���p
	enum class Select {
		title,
		stageSelect,
		size,
	};

	// �I����
	Select selectNum;
	// �I�����s���Ă���
	bool onSelect;

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

