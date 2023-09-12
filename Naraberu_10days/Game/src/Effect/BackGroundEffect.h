#include "KuroEngineDevice.h"
#include"ForUser/Timer.h"
#include<memory>

class ParticleEmitter;

class BackGroundEffect
{
private:
	static const int MAX_NUM = 576;
	std::shared_ptr<ParticleEmitter>m_ptEmitter;
	std::array<KuroEngine::Vec2<float>, MAX_NUM>m_emitPosArray;
	KuroEngine::Timer m_timer;

public:
	BackGroundEffect();

	void Initialize()
	{
		m_timer.Reset(30.0f);
	}

	void Update();
};