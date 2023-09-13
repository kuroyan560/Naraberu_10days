#pragma once
#include"ForUser/Timer.h"
#include<memory>
#include<array>
#include"ForUser/ImpactShake.h"
#include"Common/Vec.h"
#include<vector>

namespace KuroEngine
{
	class TextureBuffer;
};
class SkillResultUI;
class ParticleEmitter;

class PerfectBonusEffect
{
	enum STATUS
	{
		STATUS_BLACK_OUT, //Γ]
		STATUS_APPEAR_PERFECT,	//Perfect\¦
		STATUS_APPEAR_BONUS,	//{[iX\¦
		STATUS_DIAPPEAR,
		STATUS_PASSIVE,
		STATUS_NUM,
	}m_nowStatus;

	KuroEngine::Timer m_timer;


	std::array<float, STATUS_NUM>m_interval;

	//Γ]
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

	std::vector<std::weak_ptr<SkillResultUI>>m_enemyDamageUIArray;
	std::weak_ptr<ParticleEmitter>m_ptEmitter;

	void OnChangeStatus();

public:
	PerfectBonusEffect();
	void Init() { m_nowStatus = STATUS_PASSIVE; }
	void Update();
	void DrawBlackOut();
	void Draw();

	void Start(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI);
	bool IsActive() { return m_nowStatus != STATUS_PASSIVE; }
};