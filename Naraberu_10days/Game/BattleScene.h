#pragma once
#include "KuroEngineDevice.h"

#include "Player.h" 
#include "Enemy.h"
#include "BattleTurnMgr.h"

class BattleScene :
    public KuroEngine::BaseScene
{

	Player Pl;
	std::vector<UnitBase*> En;

	BattleTurnMgr Mgr;

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	BattleScene();
};

