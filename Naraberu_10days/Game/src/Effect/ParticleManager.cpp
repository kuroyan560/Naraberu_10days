#include "ParticleManager.h"
#include"ParticleBase.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::Init()
{
	for (auto& ptArray : m_particles)
	{
		ptArray->Init();
	}
}

void ParticleManager::Update()
{
	for (auto& ptArray : m_particles)
	{
		ptArray->Update();
	}
}

void ParticleManager::Draw(int arg_layer)
{
	for (auto& ptArray : m_particles)
	{
		if (ptArray->GetLayer() != arg_layer)continue;
		ptArray->Draw();
	}
}

void ParticleManager::ParticleArray::Init()
{
	for (auto& pt : m_aliveParticles)
	{
		pt->Init();
	}
	m_aliveParticles.clear();
}

void ParticleManager::ParticleArray::Update()
{
	auto before = m_aliveParticles.before_begin();

	for (auto itr = m_aliveParticles.begin(); itr != m_aliveParticles.end();)
	{
		(*itr)->Update();

		if (!(*itr)->IsAlive())
		{
			m_deadParticle.push_front((*itr));
			itr = m_aliveParticles.erase_after(before);
		}
		else
		{
			before = itr;
			++itr;
		}
	}
}

void ParticleManager::ParticleArray::Draw()
{
	for (auto itr = m_aliveParticles.begin(); itr != m_aliveParticles.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

void ParticleManager::ParticleArray::Emit(KuroEngine::Vec2<float> arg_pos, int arg_emitNum, ParticleCustomParameter* arg_customParams)
{
	if (m_deadParticle.empty())return;
	if (std::distance(m_deadParticle.begin(), m_deadParticle.end()) < arg_emitNum)return;

	int emitCount = 0;

	auto before = m_deadParticle.before_begin();
	for (auto itr = m_deadParticle.begin(); itr != m_deadParticle.end();)
	{
		if (!(*itr)->IsAlive())
		{
			(*itr)->Emit(arg_pos, arg_customParams);
			emitCount++;
			m_aliveParticles.push_front((*itr));
			itr = m_deadParticle.erase_after(before);
		}
		else
		{
			before = itr;
			++itr;
		}

		if (arg_emitNum <= emitCount)break;
	}
}
