#pragma once
#include"ParticleBase.h"
#include"Common/Vec.h"
#include<memory>
#include"ForUser/Timer.h"
#include<vector>

class ParticleEmitter;

namespace KuroEngine
{
	class TextureBuffer;
}

class UltActivateEffect
{
	std::shared_ptr<KuroEngine::TextureBuffer>m_goldPrismTex;

	KuroEngine::Timer m_timer;
	KuroEngine::Timer m_putTimer;

	bool m_isActive = false;
	std::weak_ptr<ParticleEmitter>m_ptEmitter;
	std::vector<KuroEngine::Vec2<float>>m_emitDestPosArray;

	KuroEngine::Timer m_waveTimer;
	float m_waveRadius = 0.0f;
	float m_waveAlpha = 0.0f;

public:
	UltActivateEffect();
	void Init();
	void Update();
	void Draw();

	void Start();
	bool GetActive() { return m_isActive; }
};