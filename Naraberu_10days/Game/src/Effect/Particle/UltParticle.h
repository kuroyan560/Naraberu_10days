#pragma once
#include"../ParticleBase.h"
#include"Common/Vec.h"
#include<memory>
#include"ForUser/Timer.h"
#include"Common/Angle.h"

namespace KuroEngine
{
	class TextureBuffer;
}

class Player;

class UltParticle : public ParticleBase
{
	static std::shared_ptr<KuroEngine::TextureBuffer>s_tex;
	static std::weak_ptr<Player>s_player;

	KuroEngine::Vec2<float>m_vel = { 0.0f,0.0f };
	KuroEngine::Vec2<float>m_accel = { 0.0f,0.0f };
	KuroEngine::Vec2<float>m_pos = { 0.0f,0.0f };
	KuroEngine::Timer m_dropTimer;

	KuroEngine::Angle m_spin;
	float m_alpha = 0.0f;
	float m_scale = 0.0f;

	bool m_fall = false;

	bool m_absorb = false;
	KuroEngine::Vec2<float>m_from;
	KuroEngine::Timer m_absorbTimer;

	void OnInit()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnEmit(KuroEngine::Vec2<float>arg_pos, ParticleCustomParameter* arg_params)override;

public:
	static void SetPlayer(std::weak_ptr<Player>arg_player)
	{
		s_player = arg_player;
	}
	UltParticle();
};

