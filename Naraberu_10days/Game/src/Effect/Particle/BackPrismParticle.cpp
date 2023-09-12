#include "BackPrismParticle.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "DirectX12/D3D12App.h"
#include"Common/Angle.h"
#include"FrameWork/WinApp.h"

std::array<std::shared_ptr<KuroEngine::TextureBuffer>, BackPrismParticle::TRIANGLE_PATTERN_NUM>BackPrismParticle::s_triangleTex;
std::array<KuroEngine::Color, BackPrismParticle::COLOR_PATTERN_NUM> BackPrismParticle::s_colorPattern =
{
	KuroEngine::Color(255,93,204,255),
	KuroEngine::Color(94,253,247,255),
	KuroEngine::Color(253,254,137,255),
};

BackPrismParticle::BackPrismParticle()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/back_ground/";

	if (!s_triangleTex[0])
	{
		for (int i = 0; i < 3; i++) 
		{
			s_triangleTex[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "triangle" + std::to_string(i + 1) + ".png");
		}
	}
}

void BackPrismParticle::OnInit()
{
}

void BackPrismParticle::OnUpdate()
{
	using namespace KuroEngine;

	if (m_timer.UpdateTimer())m_isAlive = false;

	const float MAX_ALPHA = 0.6f;
	m_alpha = sin(m_timer.GetTimeRate() * Angle::PI()) * MAX_ALPHA;
	const float MOVE_SPEED = 1.0f;
	m_pos += m_moveVec * MOVE_SPEED;

	const float SPIN_SPEED = Angle::ConvertToRadian(0.5f);
	m_angle += SPIN_SPEED * static_cast<float>(m_spinVec);
}

void BackPrismParticle::OnDraw()
{
	KuroEngine::Color col = s_colorPattern[m_colorIdx];
	col = KuroEngine::Color(76, 96, 170, 255);
	col.m_a *= m_alpha;
	KuroEngine::DrawFunc2D_Color::DrawRotaGraph2D(m_pos, m_scale, m_angle, s_triangleTex[m_texNum], col);
}

void BackPrismParticle::OnEmit(KuroEngine::Vec2<float> arg_pos)
{
	m_scale = { KuroEngine::GetRand(0.2f, 0.4f),KuroEngine::GetRand(0.2f, 0.4f) };
	m_angle = KuroEngine::Angle(KuroEngine::GetRand(360));
	m_texNum = KuroEngine::GetRand(TRIANGLE_PATTERN_NUM);
	m_timer.Reset(300);
	m_colorIdx = KuroEngine::GetRand(COLOR_PATTERN_NUM);

	m_moveVec = { cos(m_angle),sin(m_angle) };
	m_pos = KuroEngine::WinApp::Instance()->GetExpandWinCenter();
	m_pos += m_moveVec * 280.0f;

	m_spinVec = KuroEngine::GetRandPlusMinus();
}
