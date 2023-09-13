#include "Player.h"
#include "FrameWork/UsersInput.h"
#include "../../OperationConfig.h"
#include "../ExistUnits.h"
#include "../Player_Act/Skills/PlayerSkills.h"

#include "../../Panel/PanelManager.h"

#include"../../Effect/ScreenShakeManager.h"

#include"../../Effect/UltActivateEffect.h"

#include"../../SoundConfig.h"

Player::Player()
{
	// ターン関連変数の初期化
	m_isMyTurn = false;
	m_NextTurn = false;

	m_IsEndBonus = true;
	m_DoBonus = false;
	m_UltimatePoint = 0;
	m_BeforeUltimatePoint = 0;
	Max_UltimatePoint = 400;
	m_Damage_Timer = 0;

	m_Player_Mabataki_Timer = 0;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_HpFrameTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_frame.png");
	m_HpTex_green = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_green.png");
	m_HpTex_yellow = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_yellow.png");
	m_HpTex_red = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_red.png");
	m_HpTex_break = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_hp_gauge_break.png");
	m_Ult_Gauge = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "ult_gauge.png");

	m_CharacterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "character/player_character_normal.png");
	m_CharacterDamageTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "character/player_character_damage.png");
	m_CharacterMabatakiTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "character/player_character_mabadaki.png");
	D3D12App::Instance()->GenerateTextureBuffer(&m_NumberTex.front(), TexDir + "player_hp_number.png", 11, Vec2(11, 1));

	TurnChangeTimer = 0;
	TurnChangeTime_Fin = int(100.0f * RefreshRate::RefreshRate_Mag);

	//ウルト演出系
	const KuroEngine::Vec2<float> size = { 57.5f ,57.5f };
	for (int i = 0; i < 4; i++) {
		// 分母0は4んじゃう；；
		if (i == 0) {
			ultSize[i] = KuroEngine::Vec2(0.0f, 0.0f);
			continue;
		}
		ultSize[i] = KuroEngine::Vec2(size.x / float(i), size.y / float(i) );
	}
}

void Player::OnInitialize()
{
	m_IsEndBonus = true;
	m_DoBonus = false;
	m_Damage_Timer = 0;
}

void Player::OnUpdate()
{
	using namespace KuroEngine;

	// タイマーの加算
	TimerUpdate();

	if (TurnChangeTimer == 1) {
		TurnEnd_BeforeTurnChange();
		TurnChangeTimer++;
	}
	if (TurnChangeTimer > 0 && ExistUnits::Instance()->m_IsEndBonusCount) {
		TurnChangeTimer++;
	}
	if (TurnChangeTimer == TurnChangeTime_Fin) {
		EndTurn();
	}

	// ウルト発動
	m_UltimatePoint = Max_UltimatePoint;
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::EXECUTE_ULT, OperationConfig::ON_TRIGGER) && GetUltRate() == 1.0f) {
		
		if (ExistUnits::Instance()->m_StageManager->GetOneSpacePosArray().empty())
		{
			SoundConfig::Instance()->Play(SoundConfig::SE_CANNOT_SELECT);
		}
		else
		{
			SubUltPoint(Max_UltimatePoint);
			m_UltimateActivateEffect.lock()->Start();
			PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Ultimate_01", 0, ExistUnits::Instance()->m_pPlayer);
		}
	}
}

void Player::OnAlwaysUpdate()
{
	using namespace KuroEngine;
	// ダメージ差分用タイマー
	if (m_Damage_Timer > 0) {
		m_Damage_Timer--;
	}
	// まばたき用タイマー
	if (m_Player_Mabataki_Timer > 0) {
		m_Player_Mabataki_Timer--;
	}
	else {
		int Mabataki_ = GetRand(0, 100);
		if (Mabataki_ == 1) {
			m_Player_Mabataki_Timer = 500;
		}
	}
	/*if (OperationConfig::Instance()->DebugKeyInput(DIK_B)) {
		m_HP > 0 ? m_HP-- : 0;
	}
	if (OperationConfig::Instance()->DebugKeyInput(DIK_N)) {
		m_HP < m_MaxHP ? m_HP++ : 0;
	}*/
}

void Player::OnDraw()
{
	using namespace KuroEngine;

	//キャラクターの描画
	if (m_Player_Mabataki_Timer > 480) {
		DrawFunc2D::DrawGraph(
			Vec2(39.0f, 66.0f) + ScreenShakeManager::Instance()->GetOffset(), m_CharacterMabatakiTex);
	}
	else {
		DrawFunc2D::DrawGraph(
			Vec2(39.0f, 66.0f) + ScreenShakeManager::Instance()->GetOffset(), m_CharacterTex);
	}

	// ダメージ差分
	if (m_Damage_Timer > 0) {
		DrawFunc2D::DrawGraph(
			Vec2(39.0f, 66.0f) + ScreenShakeManager::Instance()->GetOffset(), m_CharacterDamageTex);
	}


	DrawFunc2D::DrawExtendGraph2D(Vec2(11.0f, 410.0f) + ScreenShakeManager::Instance()->GetOffset(),
		Vec2(368.0f, 595.0f) + ScreenShakeManager::Instance()->GetOffset(), m_HpFrameTex);
	//DrawFunc2D::DrawExtendGraph2D(Vec2(22.0f, 427.0f), Vec2(357.0f, 580.0f), m_HpTex);

	// HPゲージが削れる演出用
	float HP_Gauge_Now_Value = float(m_HP);

	// タイマーが1以上　かつ　最大値以下なら減少演出
	float Shake = 0.0f;
	if (m_HP_Break_Timer > 0 && m_HP_Break_Timer < m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer++;
		HP_Gauge_Now_Value = float(m_HP) + float(m_Before_HP - m_HP) * (1.0f - (float(m_HP_Break_Timer) / float(m_HP_GAUGE_BREAK_TIME)));
		Shake = float(m_HP_Break_Timer % 5);
	}
	// 減少演出終わり
	if (m_HP_Break_Timer == m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer = 0;
	}
	//if(m_HP < m_Before_HP)
	//m_HP_GAUGE_BREAK_TIME

	// 現在のHP割合
	float Now_HP_Rate = float(HP_Gauge_Now_Value) / float(m_MaxHP);
	// 現在のHP割合2
	float Now_HP_Rate2 = float(m_HP) / float(m_MaxHP);
	// HPゲージの長さ
	float Gauge_Max_Width = 357.0f - 22.0f;
	// 現在のHPゲージの長さ
	float Gauge_Width = Gauge_Max_Width * Now_HP_Rate;
	float Gauge_Width2 = Gauge_Max_Width * Now_HP_Rate2;

	std::shared_ptr<KuroEngine::TextureBuffer> HP_Gauge = m_HpTex_green;
	if (Now_HP_Rate <= 0.2f) {
		HP_Gauge = m_HpTex_red;
	}
	else if (Now_HP_Rate <= 0.5f) {
		HP_Gauge = m_HpTex_yellow;
	}

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(22.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset(), Vec2(357.0f, 580.0f) + ScreenShakeManager::Instance()->GetOffset(), m_HpTex_break,
		Vec2(22.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset(), Vec2(22.0f + Gauge_Width, 580.0f) + ScreenShakeManager::Instance()->GetOffset());

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(22.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset(), Vec2(357.0f, 580.0f) + ScreenShakeManager::Instance()->GetOffset(), HP_Gauge,
		Vec2(22.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset(), Vec2(22.0f + Gauge_Width2, 580.0f) + ScreenShakeManager::Instance()->GetOffset());

	// HPの数値描画
	DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(m_MaxHP, 0, false), Vec2(333.0f + 5.0f, 489.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	if (m_MaxHP > 9) {// 2桁目の描画
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(m_MaxHP, 1, false), Vec2(303.0f + 5.0f, 478.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}

	DrawFunc2D::DrawGraph(Vec2(252.0f - 7.0f, 461.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), m_NumberTex[10]);

	if (m_MaxHP > 99) {// 3桁目の描画
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(m_MaxHP, 2, false), Vec2(270.0f + 5.0f, 468.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}
	

	// 1桁目の描画
	DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(int(HP_Gauge_Now_Value), 0, false), Vec2(222.0f - 5.0f, 447.0f - 2.0f - Shake) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	if (int(HP_Gauge_Now_Value) > 9) {// 2桁目の描画
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(int(HP_Gauge_Now_Value), 1, false), Vec2(192.0f - 5.0f, 436.0f - 2.0f - Shake) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}
	if (int(HP_Gauge_Now_Value) > 99) {// 3桁目の描画
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(int(HP_Gauge_Now_Value), 2, false), Vec2(162.0f - 5.0f, 426.0f - 2.0f - Shake) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}

	// アルティメットゲージの描画
	// 現在のゲージ割合
	float Now_ULT_Rate = float(GetUltPoint()) / float(Max_UltimatePoint);
	// ゲージの高さ
	float Gauge_ULT_Max_Width = 129.0f - 14.0f;
	// 現在のゲージの高さ
	float Gauge_ULT_Width = Gauge_ULT_Max_Width * Now_ULT_Rate;
	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(248.0f, 14.0f) + ScreenShakeManager::Instance()->GetOffset(), Vec2(363.0f, 129.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Ult_Gauge,
		Vec2(248.0f, 129.0f - Gauge_ULT_Width) + ScreenShakeManager::Instance()->GetOffset(), Vec2(363.0f, 129.0f) + ScreenShakeManager::Instance()->GetOffset());

	if (GetUltRate() == 1.0f) {
		UltMaxEffect();
		const KuroEngine::Vec2<float> ultPos = { 305.5f,71.5f };
		for (auto& i : ultSize) {
			DrawFunc2D::DrawExtendGraph2D(Vec2( ultPos.x - i.x,ultPos.y - i.y ) + ScreenShakeManager::Instance()->GetOffset(),
				Vec2( ultPos.x + i.x,ultPos.y + i.y ) + ScreenShakeManager::Instance()->GetOffset(), m_Ult_Gauge, 0.1f, AlphaBlendMode::AlphaBlendMode_Add);
		}
	}

	// 描画位置の保存
	m_Left_Top = Vec2(32.0f, 60.0f);
	m_Right_Bottom = Vec2(354.0f, 610.0f);
	m_Center = m_Left_Top + ((m_Right_Bottom - m_Left_Top) / 2.0f);
}

void Player::OnImguiDebug()
{
}

void Player::OnFinalize()
{
}

void Player::Damage(int value)
{
	// HPが削れていく演出用
	m_Before_HP = m_HP;
	m_HP_Break_Timer = 1;
	// ダメージ
	m_HP -= value;
	if (m_HP < 0) {
		m_HP = 0;
	}

	// タイマー
	m_Damage_Timer = int(40.0f * RefreshRate::RefreshRate_Mag);

	// チュートリアルなら死なない
	if (ExistUnits::Instance()->m_StageName == "Tutorial") {
		if (m_HP <= 0) {
			m_HP = 1;
		}
	}
}

void Player::TurnEnd_BeforeTurnChange()
{
	m_IsEndTurnFunc = true;
	// ここにボーナスアタックとか書く
	// マップをリセット
	m_DoBonus = true;
	ExistUnits::Instance()->m_IsEndBonusCount = false;
	ExistUnits::Instance()->m_StageManager->SetBouns();
	OperationConfig::Instance()->SetInGameOperationActive(false);
	// ボーナス
	m_IsEndBonus = false;
}

void Player::SetState(int HP, int MaxHP)
{
	m_HP = HP;
	m_Before_HP = 0;
	m_MaxHP = MaxHP;
	m_UltimatePoint = 0;
	m_BeforeUltimatePoint = 0;
	Max_UltimatePoint = 100;
	m_IsEndTurnFunc = false;
	m_DoBonus = false;
}

void Player::UltMaxEffect()
{
	//基礎サイズ
	const KuroEngine::Vec2<float> size = { 57.5f ,57.5f };

	for (auto& i : ultSize) {
		i.x += 0.2f;
		i.y += 0.2f;
		if (i.x > 70.0f) {
			i = size;
		}
	}
}

void Player::TurnEndTrigger()
{
	TurnChangeTimer = 1;
	//EndTurn();
}
