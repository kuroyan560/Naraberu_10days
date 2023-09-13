#pragma once
#include"ForUser/Timer.h"
#include<memory>
#include<array>
#include"ForUser/ImpactShake.h"
#include"Common/Vec.h"

namespace KuroEngine
{
	class TextureBuffer;
};

class PerfectBonusEffect
{
	enum STATUS
	{
		STATUS_BLACK_OUT, //暗転
		STATUS_APPEAR_PERFECT,	//Perfect表示
		STATUS_APPEAR_BONUS,	//ボーナス表示
		STATUS_DIAPPEAR,
		STATUS_PASSIVE,
		STATUS_NUM,
	}m_nowStatus;

	KuroEngine::Timer m_timer;


	std::array<float, STATUS_NUM>m_interval;

	//暗転
	float m_blackOutAlpha = 0.0f;

	struct Content
	{
		bool m_draw;
		KuroEngine::Vec2<float>m_pos;
		float m_scale = 1.0f;
		float m_alpha = 0.0f;
		std::shared_ptr<KuroEngine::TextureBuffer>m_tex;
	};

	Content m_perfect;
	KuroEngine::ImpactShake m_perfectImpactShake;

	Content m_bonus;

	void OnChangeStatus();

public:
	PerfectBonusEffect();
	void Init() { m_nowStatus = STATUS_PASSIVE; }
	void Update();
	void Draw();

	void Start();
};