#pragma once
#include "KuroEngineDevice.h"

class TitleScene : public KuroEngine::BaseScene
{
	// 背景
	std::shared_ptr<KuroEngine::TextureBuffer> backTex;
	//タイトル
	std::shared_ptr<KuroEngine::TextureBuffer> titleTex;
	//選択肢
	std::shared_ptr<KuroEngine::TextureBuffer> stageTex;
	//数字
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,10> numTex;

	//選択肢番号
	int stageNum;


	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	TitleScene();

};

