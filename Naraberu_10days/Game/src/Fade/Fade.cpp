#include "Fade.h"
#include "FrameWork/UsersInput.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "FrameWork/WinApp.h"
#include "../../src/BattleManager/ExistUnits.h"
#include "../RefreshRate.h"
#include"../OperationConfig.h"


void Fade::OnStart()
{
	FadeMode = false;
	m_Timer = 0;
	m_FinishTime = int(20.0f * RefreshRate::RefreshRate_Mag);
}

bool Fade::Update()
{
	m_Timer++;
	if (!FadeMode) {
		if (m_Timer == m_FinishTime) {
			FadeMode = true;
			m_Timer = 0;
			return true;
		}
	}
	else {
		if (m_Timer == m_FinishTime) {
			m_nowTrans = false;
		}
	}

	return false;
}

void Fade::Draw()
{
	using namespace KuroEngine;
	Vec2<float> WinSize = WinApp::Instance()->GetExpandWinSize();
	if (!FadeMode) {
		DrawFunc2D::DrawBox2D(Vec2(WinSize.x - float(OutExpo((float)m_Timer / (float)m_FinishTime)) * float(WinSize.x), 0.0f),
			WinSize, Color(10, 10, 10, 255), true);
	}
	else {
		DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f),
			Vec2(WinSize.x - float(OutExpo((float)m_Timer / (float)m_FinishTime)) * WinSize.x, WinSize.y), Color(10, 10, 10, 255), true);
	}
}
