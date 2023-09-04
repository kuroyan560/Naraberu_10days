#pragma once
#include "UnitBase.h"
class Player :
    public UnitBase
{
private:
    int HP = 1;

public:
	Player();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
};

