#pragma once
#include "KuroEngineDevice.h"
class BattleScene :
    public KuroEngine::BaseScene
{
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	BattleScene();
};

