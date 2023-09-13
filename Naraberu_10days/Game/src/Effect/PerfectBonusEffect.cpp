#include "PerfectBonusEffect.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"FrameWork/WinApp.h"
#include"../RefreshRate.h"
#include"DirectX12/D3D12App.h"

void PerfectBonusEffect::OnChangeStatus()
{
	switch (m_nowStatus)
	{
		case STATUS_APPEAR_PERFECT:
			m_perfectImpactShake.Shake(m_interval[STATUS_APPEAR_PERFECT], 2.0f, 2.0f, 32.0f);
			m_perfect.m_draw = true;
			return;
		case STATUS_APPEAR_BONUS:
			m_bonus.m_draw = true;
			return;
		case STATUS_DIAPPEAR:
			return;
	}
}

PerfectBonusEffect::PerfectBonusEffect()
{
	using namespace KuroEngine;

	std::string dir = "resource/user/tex/battle_scene/perfect_bonus/";

	m_perfect.m_tex = D3D12App::Instance()->GenerateTextureBuffer(dir + "perfect.png");
	m_bonus.m_tex = D3D12App::Instance()->GenerateTextureBuffer(dir + "bonus.png");
}

void PerfectBonusEffect::Update()
{
	using namespace KuroEngine;

	if (m_nowStatus == STATUS_PASSIVE)return;

	if (m_timer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))
	{
		m_nowStatus = STATUS((int)m_nowStatus + 1);
		if (m_nowStatus == STATUS_NUM)
		{
			m_nowStatus = STATUS_PASSIVE;
			return;
		}
		m_timer.Reset(m_interval[m_nowStatus]);
		OnChangeStatus();
	}

	const float BLACK_OUT_ALPHA = 0.4f;
	const Vec2<float>PERFECT_DEF_POS = { 648.0f,287.0f };
	const Vec2<float>BONUS_DEF_POS = { 693.0f,424.0f };
	const float DISAPPEAR_OFFSET_Y = 100.0f;

	switch (m_nowStatus)
	{
		case STATUS_BLACK_OUT:
			m_blackOutAlpha = Math::Lerp(0.0f, BLACK_OUT_ALPHA, m_timer.GetTimeRate(0.5f));
			return;
		case STATUS_APPEAR_PERFECT:
			m_perfect.m_pos = PERFECT_DEF_POS;
			m_perfect.m_scale = Math::Ease(Out, Exp, m_timer.GetTimeRate(0.5f), 3.0f, 1.0f);
			m_perfect.m_alpha = Math::Ease(Out, Exp, m_timer.GetTimeRate(0.5f), 0.0f, 1.0f);
			m_perfectImpactShake.Update(1.0f / RefreshRate::RefreshRate_Mag);
			return;
		case STATUS_APPEAR_BONUS:
			m_bonus.m_pos = BONUS_DEF_POS;
			m_bonus.m_pos.y = Math::Ease(Out, Back, m_timer.GetTimeRate(0.4f), BONUS_DEF_POS.y - 32.0f, BONUS_DEF_POS.y);
			m_bonus.m_scale = Math::Ease(Out, Back, m_timer.GetTimeRate(0.3f), 0.0f, 1.0f);
			m_bonus.m_alpha = Math::Ease(Out, Quint, m_timer.GetTimeRate(0.4f), 0.0f, 1.0f);
			return;
		case STATUS_DIAPPEAR:
			m_perfect.m_pos.y = Math::Ease(In, Back, m_timer.GetTimeRate(), PERFECT_DEF_POS.y, PERFECT_DEF_POS.y + DISAPPEAR_OFFSET_Y);
			m_perfect.m_alpha = Math::Ease(In, Quart, m_timer.GetTimeRate(), 1.0f, 0.0f);
			m_bonus.m_pos.y = Math::Ease(In, Back, m_timer.GetTimeRate(), BONUS_DEF_POS.y, BONUS_DEF_POS.y + DISAPPEAR_OFFSET_Y);
			m_bonus.m_alpha = Math::Ease(In, Quart, m_timer.GetTimeRate(), 1.0f, 0.0f);
			m_blackOutAlpha = Math::Ease(In, Quart, m_timer.GetTimeRate(), BLACK_OUT_ALPHA, 0.0f);
			return;
	}

}

void PerfectBonusEffect::Draw()
{
	using namespace KuroEngine;

	if (m_nowStatus == STATUS_PASSIVE)return;

	Vec2 Panel_LT = Vec2(391.0f, 67.0f);
	Vec2 Panel_RB = Vec2(892.0f, 565.0f);

	// ƒpƒlƒ‹‚ðˆÃ‚­
	DrawFunc2D::DrawBox2D(Panel_LT, Panel_RB, Color(0.0f, 0.0f, 0.0f, m_blackOutAlpha), true);

	if (m_perfect.m_draw)
	{
		Vec2<float>shake = { m_perfectImpactShake.GetOffset().x,m_perfectImpactShake.GetOffset().y };
		DrawFunc2D::DrawRotaGraph2D(m_perfect.m_pos + shake, {m_perfect.m_scale,m_perfect.m_scale}, 0.0f, m_perfect.m_tex, m_perfect.m_alpha);
	}
	if (m_bonus.m_draw)
	{
		DrawFunc2D::DrawRotaGraph2D(m_bonus.m_pos, { m_bonus.m_scale,m_bonus.m_scale }, 0.0f, m_bonus.m_tex, m_bonus.m_alpha);
	}
}

void PerfectBonusEffect::Start()
{
	m_nowStatus = STATUS_BLACK_OUT;
	m_interval =
	{
		60.0f,
		90.0f,
		120.0f,
		60.0f,
		1.0f,
	};

	m_timer.Reset(m_interval[m_nowStatus]);
	m_blackOutAlpha = 0.0f;

	m_perfect.m_draw = false;
	m_bonus.m_draw = false;
}