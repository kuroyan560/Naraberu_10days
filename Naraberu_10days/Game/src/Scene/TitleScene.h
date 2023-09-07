#pragma once
#include "KuroEngineDevice.h"

class TitleScene : public KuroEngine::BaseScene
{
	// �w�i
	std::shared_ptr<KuroEngine::TextureBuffer> backTex;
	//�^�C�g��
	std::shared_ptr<KuroEngine::TextureBuffer> titleTex;
	//�I����
	std::shared_ptr<KuroEngine::TextureBuffer> stageTex;
	//����
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,10> numTex;

	//�I�����ԍ�
	int stageNum;


	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	TitleScene();

};

