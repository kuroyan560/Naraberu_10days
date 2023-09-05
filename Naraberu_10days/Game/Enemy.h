#pragma once
#include "UnitBase.h"
#include "EnemysData.h"

#include "EnemyActList.h"

#include <memory>

class Enemy :
    public UnitBase
{
private:
	EnemysData::EnemyData m_Data;
	//std::vector<std::shared_ptr<E_Ac_Base>> m_Actions;
	int m_Action_Num;
	bool m_Now_Action;
	bool m_Already_Act;

	int m_Timer;
	
public:
	Enemy();

public:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnAlwaysUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

	void SetEnemyData(EnemysData::EnemyData Data);

	EnemysData::EnemyData GetEnemyData() { return m_Data; }

	// •`‰æ
	void Draw(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn);
};

