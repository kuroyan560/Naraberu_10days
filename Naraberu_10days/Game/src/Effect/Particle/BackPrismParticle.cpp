#include "BackPrismParticle.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"Common/Color.h"

void BackPrismParticle::OnInit()
{
}

void BackPrismParticle::OnUpdate()
{
	if (m_timer.UpdateTimer())m_isAlive = false;

	m_pos.x += m_move;
}

void BackPrismParticle::OnDraw()
{
	KuroEngine::Vec2<float>size = { 32.0f,32.0f };
	KuroEngine::DrawFunc2D::DrawBox2D(m_pos - size, m_pos + size,
		KuroEngine::Color(0.0f, 0.0f, 1.0f, 1.0f - m_timer.GetTimeRate()), true);
}

void BackPrismParticle::OnEmit(KuroEngine::Vec2<float> arg_pos)
{
	m_timer.Reset(60);
	m_pos = arg_pos;
	m_move = KuroEngine::GetRand(-5.0f, 5.0f);
}
