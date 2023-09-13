#include "UltActivateEffect.h"
#include"ParticleManager.h"
#include"Particle/GoldPrismParticle.h"
#include"../BattleManager/ExistUnits.h"
#include"../Panel/PanelManager.h"
#include"../RefreshRate.h"
#include"../SoundConfig.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"

UltActivateEffect::UltActivateEffect()
{
	m_ptEmitter = ParticleManager::Instance()->Register<GoldPrismParticle>(200, false);
	m_goldPrismTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/block/block_gold.png");
}

void UltActivateEffect::Init()
{
	m_isActive = false;
}

void UltActivateEffect::Update()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	if (m_timer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))
	{
		if (m_putTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))
		{
			m_isActive = false;
		}
	}

	if (m_timer.IsTimeUpOnTrigger())SoundConfig::Instance()->Play(SoundConfig::SE_PUT_GOLD, 2);

	const Vec2<float>from = { 305.0f,71.0f };

	for (auto& destPos : m_emitDestPosArray)
	{
		auto emitPos = Math::Lerp(from, destPos, m_timer.GetTimeRate());
		m_ptEmitter.lock()->Emit(emitPos, GetRand(1, 3));
	}

	m_waveTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag);
	m_waveRadius = Math::Ease(Out, Circ, m_waveTimer.GetTimeRate(), 0.0f, 500.0f);
	m_waveAlpha = Math::Ease(Out, Circ, m_waveTimer.GetTimeRate(), 1.0f, 0.0f);
}

void UltActivateEffect::Draw()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	KuroEngine::Color color = { 255,177,8,255 };
	color.m_a = m_waveAlpha;
	for (int i = 0; i < 5; i++) {
		DrawFunc2D::DrawCircle2D({ 305.0f,71.0f }, m_waveRadius - i * 32, color, 
			false, int((12 + i * 2) * m_waveTimer.GetTimeRate()), KuroEngine::AlphaBlendMode::AlphaBlendMode_Add);
	}

	if (m_timer.IsTimeUp())
	{
		float scale = Math::Ease(Out, Elastic, m_putTimer.GetTimeRate(), 2.0f, 1.0f);
		float spin = Math::Ease(Out, Circ, m_putTimer.GetTimeRate(), Angle::PI(), 0.0f);
		float alpha = Math::Ease(Out, Circ, m_putTimer.GetTimeRate(), 1.0f, 0.0f);

		for (auto& putPos : m_emitDestPosArray)
		{
			DrawFunc2D::DrawRotaGraph2D(putPos, 
				{ scale,scale }, spin, m_goldPrismTex, alpha, { 0.5f,0.5f }, AlphaBlendMode_Add);
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

	SoundConfig::Instance()->Play(SoundConfig::SE_ACTIVATE_ULT, 2);

	m_waveTimer.Reset(45.0f);
	m_waveRadius = 0.0f;
	m_waveAlpha = 1.0f;

	m_putTimer.Reset(30.0f);
}