#pragma once
#include "KuroEngineDevice.h"

class TitleScene : public KuroEngine::BaseScene
{
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	TitleScene();

};

