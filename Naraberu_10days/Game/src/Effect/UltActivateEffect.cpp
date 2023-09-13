#include "UltActivateEffect.h"
#include"ParticleManager.h"
#include"Particle/GoldPrismParticle.h"

UltActivateEffect::UltActivateEffect()
{
	m_ptEmitter = ParticleManager::Instance()->Register<GoldPrismParticle>(200, false);
}

void UltActivateEffect::Init()
{
}

#include"../OperationConfig.h"
void UltActivateEffect::Update()
{
	static int timer = 0;
	static float x = 0.0f;

	if (OperationConfig::Instance()->DebugKeyInput(DIK_M) && (++timer % 3 == 0))
	{
		m_ptEmitter.lock()->Emit({ x,300.0f }, 1);
		if (1280.0f < (x += 30.0f))x = 0.0f;
	}
}
