#include "GoldPrismParticle.h"
#include"Common/Angle.h"
#include"KuroEngine.h"
#include"../../RefreshRate.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
std::array<std::shared_ptr<KuroEngine::TextureBuffer>, GoldPrismParticle::TEX_NUM>GoldPrismParticle::s_tex;

void GoldPrismParticle::OnInit()
{
}

void GoldPrismParticle::OnUpdate()
{
	using namespace KuroEngine;

	if (m_timer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))m_isAlive = false;

	m_pos = m_emitPos + m_moveVec * Math::Ease(Out, Quint, m_timer.GetTimeRate(), 0.0f, m_emitPower);
}

void GoldPrismParticle::OnDraw()
{
	using namespace KuroEngine;
	float alpha = Math::Ease(Out, Circ, m_timer.GetTimeRate(), 1.0f, 0.0f);
	DrawFunc2D::DrawRotaGraph2D(m_pos, { 1.0f,1.0f }, 0.0f, s_tex[m_texIdx], alpha,
		{ 0.5f,0.5f }, AlphaBlendMode_Add);
}

void GoldPrismParticle::OnEmit(KuroEngine::Vec2<float> arg_pos, ParticleCustomParameter* arg_params)
{
	m_timer.Reset(45);
	m_emitPos = arg_pos;
	m_pos = arg_pos;

	KuroEngine::Angle angle = KuroEngine::Angle::ConvertToRadian(KuroEngine::GetRand(360.0f));
	m_moveVec = { cos(angle),sin(angle) };

	m_emitPower = KuroEngine::GetRand(7.0f, 32.0f);
	m_texIdx = std::min(TEX_NUM - 1, KuroEngine::GetRand(TEX_NUM));
}

GoldPrismParticle::GoldPrismParticle()
{
	if (!s_tex[0])
	{
		KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(s_tex.data(), "resource/user/tex/particle/gold.png", TEX_NUM, { TEX_NUM,1 });
	}
}