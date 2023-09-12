#include "../Effect/BackGroundEffect.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include"Particle/BackPrismParticle.h"
#include"ParticleManager.h"

BackGroundEffect::BackGroundEffect()
{
	using namespace KuroEngine;

	KuroEngine::Vec2<float> range = { -10.0f,-10.0f };
	int count = 0;
	const float addRange = 40.0f;
	for (auto& emitPos : m_emitPosArray) {
		emitPos = { KuroEngine::GetRand(range.x,range.x + addRange),KuroEngine::GetRand(range.y, range.y + addRange) };
		count++;
		if (count % 2 == 0)
		{
			range.x += addRange;
			if (range.x >= 1290.0f) {
				range.x = -10.0f;
				range.y += addRange;
			}
		}
	}

	m_ptEmitter = ParticleManager::Instance()->Register<BackPrismParticle>(MAX_NUM, true);
}

void BackGroundEffect::Update()
{
	if (m_timer.UpdateTimer())
	{
		m_ptEmitter->Emit(m_emitPosArray[KuroEngine::GetRand(MAX_NUM - 1)], KuroEngine::GetRand(1,3));
		m_timer.Reset();
	}
}
