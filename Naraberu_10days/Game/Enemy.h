#pragma once
#include "UnitBase.h"
#include "EnemysData.h"

class Enemy :
    public UnitBase
{
private:
	EnemysData::EnemyData m_Data;

public:
	Enemy();

public:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

	void SetEnemyData(EnemysData::EnemyData Data);

	EnemysData::EnemyData GetEnemyData() { return m_Data; }
};

