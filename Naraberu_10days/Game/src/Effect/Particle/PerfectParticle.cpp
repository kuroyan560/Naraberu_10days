#include "PerfectParticle.h"
#include"Common/Angle.h"
#include"KuroEngine.h"
#include"../../RefreshRate.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"

std::array<std::shared_ptr<KuroEngine::TextureBuffer>, PerfectParticle::TEX_NUM>PerfectParticle::s_tex;

void PerfectParticle::OnInit()
{
}

void PerfectParticle::OnUpdate()
{
	using namespace KuroEngine;

	if (m_timer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))m_isAlive = false;

	m_pos = m_emitPos + m_moveVec * Math::Ease(Out, Quint, m_timer.GetTimeRate(), 0.0f, m_emitPower);

	m_radian += float(m_spinPlusMinus) * Angle::ConvertToRadian(1.0f);
}

void PerfectParticle::OnDraw()
{
	using namespace KuroEngine;
	float alpha = Math::Ease(Out, Circ, m_timer.GetTimeRate(), 1.0f, 0.0f);

	DrawFunc2D::DrawRotaGraph2D(m_pos, { m_scale,m_scale }, m_radian, s_tex[m_texIdx], alpha,
		{ 0.5f,0.5f }, AlphaBlendMode_Add);
}

void PerfectParticle::OnEmit(KuroEngine::Vec2<float> arg_pos, ParticleCustomParameter* arg_params)
{
	using namespace KuroEngine;

	m_timer.Reset(60);

	KuroEngine::Vec2<float>randOffset = { GetRand(-220.0f,220.0f),GetRand(-64.0f,64.0f) };
	arg_pos += randOffset;

	m_emitPos = arg_pos;
	m_pos = arg_pos;

	KuroEngine::Angle angle = KuroEngine::Angle::ConvertToRadian(KuroEngine::GetRand(45.0f,135.0f));
	angle += float(KuroEngine::GetRandPlusMinus()) * KuroEngine::Angle::ConvertToRadian(90.0f);

	m_moveVec = { cos(angle),sin(angle) };
	m_moveVec = (m_emitPos - Vec2<float>(640.0f, 360.0f)).GetNormal();

	m_radian = angle;

	m_emitPower = KuroEngine::GetRand(64.0f, 300.0f);
	m_texIdx = std::min(TEX_NUM - 1, KuroEngine::GetRand(TEX_NUM));
	m_spinPlusMinus = KuroEngine::GetRandPlusMinus();

	m_scale = KuroEngine::GetRand(32.0f, 64.0f);
}

PerfectParticle::PerfectParticle()
{
	using namespace KuroEngine;

	if (!s_tex[0])
	{
		s_tex[0] = D3D12App::Instance()->GenerateTextureBuffer(Color(255, 99, 0, 255));
		s_tex[1] = D3D12App::Instance()->GenerateTextureBuffer(Color(255, 205, 104, 255));
		s_tex[2] = D3D12App::Instance()->GenerateTextureBuffer(Color(255, 177, 8, 255));
	}
}