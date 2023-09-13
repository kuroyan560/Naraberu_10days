#pragma once
#include"Common/Vec.h"
#include<type_traits>

class ParticleCustomParameter
{
private:
	ParticleCustomParameter() {};

public:
	template<typename _T>
	_T* Get() 
	{
		if (std::is_same<decltype(*this), _T>::value == false)
		{
			return nullptr;
		}
		return std::dynamic_pointer_cast<_T*>(this);
	}
};

class ParticleBase
{
protected:
	bool m_isAlive = false;

	ParticleBase() {}

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnEmit(KuroEngine::Vec2<float>arg_pos, ParticleCustomParameter* arg_params) = 0;

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
	void Emit(KuroEngine::Vec2<float>arg_pos, ParticleCustomParameter* arg_params = nullptr)
	{
		m_isAlive = true;
		OnEmit(arg_pos, arg_params);
	}

	const bool& IsAlive()const { return m_isAlive; }
};

