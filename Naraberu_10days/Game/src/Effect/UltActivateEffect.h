#pragma once
#include"ParticleBase.h"
#include"Common/Vec.h"
#include<memory>

class ParticleEmitter;

class UltActivateEffect
{
	std::weak_ptr<ParticleEmitter>m_ptEmitter;

public:
	UltActivateEffect();
	void Init();
	void Update();
};