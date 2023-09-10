#pragma once
#include"Common/Singleton.h"
#include"Common/Vec.h"
#include<memory>
#include<type_traits>
#include<vector>
#include<forward_list>
#include"ParticleBase.h"

//パーティクルを出すための窓口
class ParticleEmitter
{
public:
	virtual void Emit(KuroEngine::Vec2<float>arg_pos, int arg_emitNum, int arg_pattern = -1) = 0;
};

class ParticleManager : public KuroEngine::DesignPattern::Singleton<ParticleManager>
{
	friend class KuroEngine::DesignPattern::Singleton<ParticleManager>;
	ParticleManager();

	class ParticleArray : public ParticleEmitter
	{
		//要素数
		int m_elem = 0;
		//配列の先頭アドレス
		std::shared_ptr<ParticleBase>m_ptData;

		//死んでいるパーティクル
		std::forward_list<ParticleBase*>m_deadParticle;

		//生きているパーティクル
		std::forward_list<ParticleBase*>m_aliveParticles;

		bool m_back = false;

		void Emit(KuroEngine::Vec2<float>arg_pos, int arg_emitNum, int arg_pattern = -1)override;

	public:
		ParticleArray(int arg_elem, size_t arg_onePtSize, std::shared_ptr<ParticleBase>arg_ptData, bool arg_back)
			:m_elem(arg_elem), m_ptData(arg_ptData), m_back(arg_back)
		{
			char* head = (char*)m_ptData.get();
			for (int i = 0; i < arg_elem; ++i)
			{
				char* pt = head + i * arg_onePtSize;
				m_deadParticle.push_front((ParticleBase*)pt);
			}
		}

		void Init();
		void Update();
		void Draw();
		const bool& GetIsBack()const { return m_back; }
	};
	std::vector<std::shared_ptr<ParticleArray>>m_particles;

public:
	~ParticleManager();
	template<class _T>
	std::shared_ptr<ParticleEmitter>Register(int arg_elem, bool arg_back)
	{
		if (std::is_base_of<ParticleBase, _T>::value == false)
		{
			assert(0);
		}
		auto pt = std::make_shared<_T[]>(arg_elem);
		auto newParticles = std::make_shared<ParticleArray>(arg_elem, sizeof(_T), std::dynamic_pointer_cast<ParticleBase>(pt), arg_back);
		m_particles.emplace_back(newParticles);

		return std::dynamic_pointer_cast<ParticleEmitter>(newParticles);
	}

	void Init();
	void Update();
	void FrontDraw();
	void BackDraw();
};

