#pragma once
#include"../ParticleBase.h"
#include"ForUser/Timer.h"
class HealParticle :public ParticleBase
{
	KuroEngine::Timer m_timer;
	KuroEngine::Vec2<float>m_pos;

	void OnInit()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnEmit(KuroEngine::Vec2<float>arg_pos, ParticleCustomParameter* arg_params)override;
public:
};

