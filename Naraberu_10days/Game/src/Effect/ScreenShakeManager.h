#pragma once
#include"Common/Singleton.h"
#include"Common/Vec.h"
#include"ForUser/ImpactShake.h"

class ScreenShakeManager : public KuroEngine::DesignPattern::Singleton<ScreenShakeManager>
{
	friend class KuroEngine::DesignPattern::Singleton<ScreenShakeManager>;

	KuroEngine::ImpactShake m_impactShake;
	KuroEngine::Vec2<float>m_offset = { 0.0f,0.0f };

public:
	void Init();
	void Update();

	void Shake();

	KuroEngine::Vec2<float>GetOffset()
	{
		return m_offset;
	}
	float GetActiveRate()const { return m_impactShake.GetTimeActiveRate(); }
};

