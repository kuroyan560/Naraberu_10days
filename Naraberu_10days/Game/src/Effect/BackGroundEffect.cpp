#include "../Effect/BackGroundEffect.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include"Particle/BackPrismParticle.h"
#include"ParticleManager.h"
#include"../RefreshRate.h"
#include"ScreenShakeManager.h"

BackGroundEffect::BackGroundEffect()
{
	using namespace KuroEngine;

	m_ptEmitter = ParticleManager::Instance()->Register<BackPrismParticle>(1200, true);
}

void BackGroundEffect::Update()
{
	if (m_timer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))
	{
		m_ptEmitter->Emit({0,0}, 1);
		m_timer.Reset(15);
	}
}
