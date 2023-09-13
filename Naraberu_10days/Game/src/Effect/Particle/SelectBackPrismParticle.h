#pragma once
#include"../ParticleBase.h"
#include"ForUser/Timer.h"
#include<array>
#include<memory>
#include"Common/Color.h"

namespace KuroEngine
{
	class TextureBuffer;
};

class SelectBackPrismParticle : public ParticleBase
{
	static const int TRIANGLE_PATTERN_NUM = 3;
	static std::array<std::shared_ptr<KuroEngine::TextureBuffer>, TRIANGLE_PATTERN_NUM> s_triangleTex;
	static const int COLOR_PATTERN_NUM = 3;

	KuroEngine::Timer m_timer;
	KuroEngine::Vec2<float>m_pos;
	KuroEngine::Vec2<float>m_scale;
	KuroEngine::Vec2<float>m_moveVec;
	float m_angle;
	int m_spinVec;
	float m_alpha;
	int m_texNum;

	void OnInit()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnEmit(KuroEngine::Vec2<float>arg_pos, ParticleCustomParameter* arg_params)override;

public:
	SelectBackPrismParticle();
};