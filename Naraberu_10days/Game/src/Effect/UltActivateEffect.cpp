#include "UltActivateEffect.h"
#include"ParticleManager.h"
#include"Particle/GoldPrismParticle.h"
#include"../BattleManager/ExistUnits.h"
#include"../Panel/PanelManager.h"
#include"../RefreshRate.h"
#include"../SoundConfig.h"

UltActivateEffect::UltActivateEffect()
{
	m_ptEmitter = ParticleManager::Instance()->Register<GoldPrismParticle>(200, false);
}

void UltActivateEffect::Init()
{
	m_isActive = false;
}

#include"../OperationConfig.h"
void UltActivateEffect::Update()
{
	if (!m_isActive)return;

	if (m_timer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))m_isActive = false;

	const KuroEngine::Vec2<float>from = { 305.0f,71.0f };

	if (std::fmod(m_timer.GetElaspedTime(), 3.0f) <= FLT_EPSILON)
	{
		for (auto& destPos : m_emitDestPosArray)
		{
			auto emitPos = KuroEngine::Math::Lerp(from, destPos, m_timer.GetTimeRate());
			m_ptEmitter.lock()->Emit(emitPos, KuroEngine::GetRand(1, 3));
		}
	}
}

void UltActivateEffect::Start()
{
	if (m_isActive)return;

	const float TOTAL_TIME = 70.0f;

	m_isActive = true;
	m_timer.Reset(TOTAL_TIME);

	m_emitDestPosArray.clear();

	for (auto& oneSpaceChip : ExistUnits::Instance()->m_StageManager->GetOneSpacePosArray())
	{
		m_emitDestPosArray.emplace_back(ExistUnits::Instance()->m_StageManager->GetChipCenterPos(oneSpaceChip));
	}

	SoundConfig::Instance()->Play(SoundConfig::SE_ACTIVATE_ULT);
}