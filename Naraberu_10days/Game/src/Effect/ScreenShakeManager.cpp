#include "ScreenShakeManager.h"

void ScreenShakeManager::Init()
{
	m_impactShake.Init();
	m_offset = { 0,0 };
}

void ScreenShakeManager::Update()
{
	m_impactShake.Update(1.0f);
	m_offset = { m_impactShake.GetOffset().x,m_impactShake.GetOffset().y };
}

void ScreenShakeManager::Shake()
{
	m_impactShake.Shake(35.0f, 2.0f, 0.0f, 35.0f);
}
