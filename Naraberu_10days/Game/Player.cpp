#include "Player.h"
#include "../../src/engine/FrameWork/UsersInput.h"


Player::Player()
{
	// ターン関連変数の初期化
	m_isMyTurn = false;
	m_NextTurn = false;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_HpFrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_frame.png");
	m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_green.png");
	m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_yellow.png");
	m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_red.png");
}

void Player::OnInitialize()
{
}

void Player::OnUpdate()
{
	using namespace KuroEngine;
	if (UsersInput::Instance()->KeyOnTrigger(DIK_SPACE)) {
		EndTurn();
	}
}

void Player::OnAlwaysUpdate()
{
	using namespace KuroEngine;
	if (UsersInput::Instance()->KeyInput(DIK_B)) {
		m_HP > 0 ? m_HP-- : 0;
	}
	if (UsersInput::Instance()->KeyInput(DIK_N)) {
		m_HP < m_MaxHP ? m_HP++ : 0;
	}
}

void Player::OnDraw()
{
	using namespace KuroEngine;

	DrawFunc2D::DrawExtendGraph2D(Vec2(11.0f, 410.0f), Vec2(368.0f, 595.0f), m_HpFrameTex);
	//DrawFunc2D::DrawExtendGraph2D(Vec2(22.0f, 427.0f), Vec2(357.0f, 580.0f), m_HpTex);

	// 現在のHP割合
	float Now_HP_Rate = float(m_HP) / float(m_MaxHP);
	// HPゲージの長さ
	float Gauge_Max_Width = 357.0f - 22.0f;
	// 現在のHPゲージの長さ
	float Gauge_Width = Gauge_Max_Width * Now_HP_Rate;


	std::shared_ptr<KuroEngine::TextureBuffer> HP_Gauge = m_HpTex_green;
	if (Now_HP_Rate <= 0.2f) {
		HP_Gauge = m_HpTex_red;
	}
	else if (Now_HP_Rate <= 0.5f) {
		HP_Gauge = m_HpTex_yellow;
	}

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(22.0f, 427.0f), Vec2(357.0f, 580.0f), HP_Gauge,
		Vec2(22.0f, 427.0f), Vec2(22.0f + Gauge_Width, 580.0f));
}

void Player::OnImguiDebug()
{
}

void Player::OnFinalize()
{
}
