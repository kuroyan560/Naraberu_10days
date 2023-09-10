#include "HealParticle.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"Common/Color.h"

void HealParticle::OnInit()
{
}

void HealParticle::OnUpdate()
{
	if (m_timer.UpdateTimer())m_isAlive = false;
}

void HealParticle::OnDraw()
{
	KuroEngine::Vec2<float>size = { 64.0f,64.0f };
	KuroEngine::DrawFunc2D::DrawBox2D(m_pos - size, m_pos + size,
		KuroEngine::Color(1.0f, 0.0f, 0.0f, 1.0f - m_timer.GetTimeRate()), true);
}

void HealParticle::OnEmit(KuroEngine::Vec2<float> arg_pos)
{
	m_timer.Reset(60);
	m_pos = arg_pos;
}
