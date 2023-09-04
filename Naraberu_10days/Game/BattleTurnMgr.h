#pragma once
#include "UnitBase.h"
#include <vector>

class BattleTurnMgr
{
	std::vector<UnitBase*> UnitList;
	int TurnNum;
public:
	void OnInitialize(UnitBase* Player, std::vector<UnitBase*> Enemys);

	void OnUpdate();
	void OnImguiDebug();
};

