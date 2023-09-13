#include "UltParticle.h"
#include"KuroEngine.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"FrameWork/WinApp.h"
#include"../../BattleManager/Player/Player.h"
#include"../../SoundConfig.h"

std::shared_ptr<KuroEngine::TextureBuffer>UltParticle::s_tex;
std::weak_ptr<Player>UltParticle::s_player;

UltParticle::UltParticle()
{
	if (!s_tex)
	{
		s_tex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer("resource/user/tex/particle/ult.png");
	}
}

void UltParticle::OnInit()
{
}

void UltParticle::OnUpdate()
{
	using namespace KuroEngine;

	if (!m_absorb)
	{
		//重力
		const float g = 0.17f;
		m_accel.y += g;

		m_vel += m_accel;

		m_pos += m_vel;


		Vec2<float>halfSize = s_tex->GetGraphSize().Float() / 2.0f;

		//下
		if (WinApp::Instance()->GetExpandWinSize().y - halfSize.y <= m_pos.y)
		{
			m_pos.y = WinApp::Instance()->GetExpandWinSize().y - halfSize.y;
			m_vel.x *= 0.45f;
			m_vel.y *= -0.4f;
			m_accel.y = 0.0f;
			m_fall = true;
		}
		//上
		else if (m_pos.y <= halfSize.y)
		{
			m_pos.y = static_cast<float>(halfSize.y);
			m_vel.x *= 0.6f;
			m_vel.y *= -0.4f;
			m_accel.y = 0.0f;
		}

		//右
		if (WinApp::Instance()->GetExpandWinSize().x - halfSize.x <= m_pos.x)
		{
			m_pos.x = WinApp::Instance()->GetExpandWinSize().x - halfSize.x;
			m_vel.y *= 0.7f;
			m_vel.x *= -0.95f;
			m_accel.x = 0.0f;
			m_fall = true;
		}
		//左
		else if (m_pos.x <= halfSize.x)
		{
			m_pos.x = static_cast<float>(halfSize.x);
			m_vel.y *= 0.7f;
			m_vel.x *= -0.95f;
			m_accel.x = 0.0f;
		}

		//空気抵抗
		m_vel.x = Math::Lerp(m_vel.x, 0.0f, 0.01f);
		m_vel.y = Math::Lerp(m_vel.y, 0.0f, 0.01f);

		if (m_dropTimer.UpdateTimer())
		{
			m_from = m_pos;
			m_absorb = true;
		}

		float spinSpeed = Math::Lerp(Angle(3), 0.0f, m_dropTimer.GetTimeRate());
		m_spin += spinSpeed;
	}
	else
	{
		if (m_absorbTimer.UpdateTimer())
		{
			m_isAlive = false;
			s_player.lock()->AddUltPoint(1);
			SoundConfig::Instance()->Play(SoundConfig::SE_CHARGE_ULT, 2);
		}

		const Vec2<float>dest = { 305.0f,71.0f };
		m_pos = Math::Lerp(m_from, dest, m_absorbTimer.GetTimeRate());
	}
}

void UltParticle::OnDraw()
{
	KuroEngine::DrawFunc2D::DrawRotaGraph2D(m_pos,
		{ m_scale,m_scale },
		m_spin,
		s_tex, m_alpha,
		{ 0.5f,0.5f },
		KuroEngine::AlphaBlendMode_Add);
}

void UltParticle::OnEmit(KuroEngine::Vec2<float> arg_pos, ParticleCustomParameter* arg_params)
{
	using namespace KuroEngine;

	const int ANGLE_MIN = 110;
	const int ANGLE_MAX = 230;
	const Vec2<float>POWER_MIN = { 64.0f,64.0f };
	const Vec2<float>POWER_MAX = { 90.0f,90.0f };

	Angle angle = Angle(GetRand(ANGLE_MIN, ANGLE_MAX));

	m_pos = arg_pos;
	m_spin = Angle(GetRand(360));

	//初期速度
	m_vel = { cos(angle),sin(angle) };
	m_vel *= GetRand(POWER_MIN,POWER_MAX);

	m_accel = { 0.0f,0.0f };

	m_absorb = false;
	m_dropTimer.Reset(GetRand(90.0f, 110.0f));
	m_absorbTimer.Reset(10.0f);

	m_alpha = GetRand(0.5f, 1.0f);
	m_scale = GetRand(0.7f, 1.1f);

	m_fall = false;
}
