#pragma once
#include"Common/Vec.h"
class ParticleBase
{
protected:
	bool m_isAlive = false;

	ParticleBase() {}

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnEmit(KuroEngine::Vec2<float>arg_pos) = 0;

public:
	virtual ~ParticleBase() {}

	void Init()
	{
		m_isAlive = false;
		OnInit();
	}
	void Update()
	{
		if (!m_isAlive)return;
		OnUpdate();
	}
	void Draw()
	{
		if (!m_isAlive)return;
		OnDraw();
	}
	void Emit(KuroEngine::Vec2<float>arg_pos)
	{
		m_isAlive = true;
		OnEmit(arg_pos);
	}

	const bool& IsAlive()const { return m_isAlive; }
};

