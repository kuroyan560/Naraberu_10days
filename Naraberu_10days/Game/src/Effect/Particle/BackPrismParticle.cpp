#include "BackPrismParticle.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "DirectX12/D3D12App.h"
#include"Common/Angle.h"
#include"FrameWork/WinApp.h"
#include"../ScreenShakeManager.h"

std::array<std::shared_ptr<KuroEngine::TextureBuffer>, BackPrismParticle::TRIANGLE_PATTERN_NUM>BackPrismParticle::s_triangleTex;

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

	if (m_timer.UpdateTimer()
		|| !InScreen(m_pos,s_triangleTex[m_texNum]->GetGraphSize().Float() * m_scale * 1.2f,WinApp::Instance()->GetExpandWinSize()))
	{
		m_isAlive = false;
	}

	float shakeEaseRate = Math::Ease(Out, Exp, ScreenShakeManager::Instance()->GetActiveRate(), 1.0f, 0.0f);

	const float MAX_ALPHA = 0.6f;
	m_alpha = sin(m_timer.GetTimeRate() * Angle::PI()) * (MAX_ALPHA + Math::Ease(In, Circ, ScreenShakeManager::Instance()->GetActiveRate(), 1.0f - MAX_ALPHA, 0.0f));

	const float MOVE_SPEED = 1.0f;
	Vec2<float>move = m_moveVec * MOVE_SPEED;

	const float SHAKE_ACCEL = 32.0f;
	move += m_moveVec * Math::Ease(Out, Exp, ScreenShakeManager::Instance()->GetActiveRate(), SHAKE_ACCEL, 0.0f);

	m_pos += move;

	const float SPIN_SPEED = Angle::ConvertToRadian(0.5f);
	m_angle += SPIN_SPEED * static_cast<float>(m_spinVec);

	m_scale += Vec2<float>(0.001f, 0.001f) * sin(m_angle);
}

void BackPrismParticle::OnDraw()
{
	KuroEngine::Color col = KuroEngine::Color(76, 96, 170, 255);
	col.m_a *= m_alpha;
	KuroEngine::DrawFunc2D_Color::DrawRotaGraph2D(m_pos, m_scale, m_angle, s_triangleTex[m_texNum], col);
}

void BackPrismParticle::OnEmit(KuroEngine::Vec2<float> arg_pos)
{
	m_alpha = 0.0f;
	m_scale = { KuroEngine::GetRand(0.1f, 0.5f),KuroEngine::GetRand(0.1f, 0.5f) };
	m_angle = KuroEngine::Angle(KuroEngine::GetRand(360.0f));
	m_texNum = std::min(TRIANGLE_PATTERN_NUM - 1, KuroEngine::GetRand(TRIANGLE_PATTERN_NUM));
	m_timer.Reset(300);

	m_moveVec = { cos(m_angle),sin(m_angle) };
	m_pos = KuroEngine::WinApp::Instance()->GetExpandWinCenter();
	m_pos += m_moveVec * 280.0f;

	m_spinVec = KuroEngine::GetRandPlusMinus();
}