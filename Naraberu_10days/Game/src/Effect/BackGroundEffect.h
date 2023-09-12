#include "KuroEngineDevice.h"
#include"ForUser/Timer.h"
#include<memory>

class ParticleEmitter;

class BackGroundEffect
{
private:
	std::shared_ptr<ParticleEmitter>m_ptEmitter;
	KuroEngine::Timer m_timer;

public:
	BackGroundEffect();

	void Initialize()
	{
		m_timer.Reset(30.0f);
	}

	void Update();
};