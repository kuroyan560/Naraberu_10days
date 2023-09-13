#pragma once
#include"../ParticleBase.h"
#include<memory>
#include"Common/Vec.h"
#include"ForUser/Timer.h"
#include<array>

namespace KuroEngine
{
	class TextureBuffer;
};

class PerfectParticle : public ParticleBase
{
	static const int TEX_NUM = 3;
	static std::array<std::shared_ptr<KuroEngine::TextureBuffer>, TEX_NUM>s_tex;

	KuroEngine::Vec2<float>m_pos;
	KuroEngine::Vec2<float>m_emitPos;
	KuroEngine::Vec2<float>m_moveVec;
	KuroEngine::Timer m_timer;
	float m_emitPower = 0.0f;
	int m_texIdx = 0;
	float m_radian = 0.0f;
	int m_spinPlusMinus;
	float m_scale = 0.0f;

	void OnInit()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnEmit(KuroEngine::Vec2<float>arg_pos, ParticleCustomParameter* arg_params)override;

public:
	PerfectParticle();
};