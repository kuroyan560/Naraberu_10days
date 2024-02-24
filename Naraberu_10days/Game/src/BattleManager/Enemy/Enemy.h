#pragma once
#include "../UnitBase.h"
#include "EnemysData.h"

#include "../Enemy_Act/EnemyActList.h"

#include <memory>

#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

class Enemy :
    public UnitBase
{
public:
	EnemysData::EnemyData m_Data;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DeathFrameTex;

private:
	//std::vector<std::shared_ptr<E_Ac_Base>> m_Actions;
	int m_Action_Num;
	bool m_Now_Action;
	bool m_Already_Act;
	int m_Timer;
	
	// 次に行う行動
	int m_NextAction;
	int m_Next_Act_Icon_Timer;
	int m_Next_Act_Icon_Timer_Max;
	int m_Icon_Size_Timer;
	float OutBack(float t) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f);
	}

	// チュートリアル用
	int m_Tutorial_Act_Count;

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

	// 描画
	void Draw(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn, int NowTarget);

	void Draw_Boss(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn);

private: // 被ダメ描画用
	float D_Move_Width;
	float D_IndexDiff;
	int D_NowTarget;
	int D_Index;
public:
	void Draw_Damage();
	void DrawHpGauge();

private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;

	void DrawActionIcon();
public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }

	void SetAction();

	int ShakeTimer;
	KuroEngine::Vec2<float> ShakeValue;
	void DamageShake();
	void ShakeUpdate();

	// ダメージを受ける
	void Damage(int value)override;

	int DeathEffTimer;
	int DeathEffEnd;
	static int m_DeathSE;
};

