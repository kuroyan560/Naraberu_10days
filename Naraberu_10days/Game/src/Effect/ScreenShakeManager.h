#pragma once
#include"Common/Singleton.h"
#include"Common/Vec.h"
class ScreenShakeManager : public KuroEngine::DesignPattern::Singleton<ScreenShakeManager>
{
	friend class KuroEngine::DesignPattern::Singleton<ScreenShakeManager>;
public:
	KuroEngine::Vec2<float>GetOffset()
	{
		return { 0,0 };
	}
};

