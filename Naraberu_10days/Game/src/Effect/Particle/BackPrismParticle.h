#pragma once
#include"../ParticleBase.h"
#include"ForUser/Timer.h"

class BackPrismParticle : public ParticleBase
{
	KuroEngine::Timer m_timer;
	KuroEngine::Vec2<float>m_pos;
	float m_move = 0.0f;

	void OnInit()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnEmit(KuroEngine::Vec2<float>arg_pos)override;
};