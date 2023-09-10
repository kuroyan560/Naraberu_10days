#pragma once
#include "../UnitBase.h"
#include "EnemysData.h"

#include "../Enemy_Act/EnemyActList.h"

#include <memory>

#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

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

	// ï`âÊ
	void Draw(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn, int NowTarget);

	void Draw_Boss(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn);

private: // îÌÉ_ÉÅï`âÊóp
	float D_Move_Width;
	float D_IndexDiff;
public:
	void Draw_Damage();
	void DrawHpGauge();

private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;
public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }
};

