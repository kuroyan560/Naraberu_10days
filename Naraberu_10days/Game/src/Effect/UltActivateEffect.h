#pragma once
#include"ParticleBase.h"
#include"Common/Vec.h"
#include<memory>
#include"ForUser/Timer.h"
#include<vector>

class ParticleEmitter;

class UltActivateEffect
{
	KuroEngine::Timer m_timer;

	bool m_isActive = false;
	std::weak_ptr<ParticleEmitter>m_ptEmitter;
	std::vector<KuroEngine::Vec2<float>>m_emitDestPosArray;

public:
	UltActivateEffect();
	void Init();
	void Update();

	void Start();
};