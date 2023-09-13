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
	// �^�[���֘A�ϐ��̏�����
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

	//�E���g���o�n
	const KuroEngine::Vec2<float> size = { 57.5f ,57.5f };
	for (int i = 0; i < 4; i++) {
		// ����0��4�񂶂Ⴄ�G�G
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

	// �^�C�}�[�̉��Z
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

	// �E���g����
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
	// �_���[�W�����p�^�C�}�[
	if (m_Damage_Timer > 0) {
		m_Damage_Timer--;
	}
	// �܂΂����p�^�C�}�[
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

	//�L�����N�^�[�̕`��
	if (m_Player_Mabataki_Timer > 480) {
		DrawFunc2D::DrawGraph(
			Vec2(39.0f, 66.0f) + ScreenShakeManager::Instance()->GetOffset(), m_CharacterMabatakiTex);
	}
	else {
		DrawFunc2D::DrawGraph(
			Vec2(39.0f, 66.0f) + ScreenShakeManager::Instance()->GetOffset(), m_CharacterTex);
	}

	// �_���[�W����
	if (m_Damage_Timer > 0) {
		DrawFunc2D::DrawGraph(
			Vec2(39.0f, 66.0f) + ScreenShakeManager::Instance()->GetOffset(), m_CharacterDamageTex);
	}


	DrawFunc2D::DrawExtendGraph2D(Vec2(11.0f, 410.0f) + ScreenShakeManager::Instance()->GetOffset(),
		Vec2(368.0f, 595.0f) + ScreenShakeManager::Instance()->GetOffset(), m_HpFrameTex);
	//DrawFunc2D::DrawExtendGraph2D(Vec2(22.0f, 427.0f), Vec2(357.0f, 580.0f), m_HpTex);

	// HP�Q�[�W�����鉉�o�p
	float HP_Gauge_Now_Value = float(m_HP);

	// �^�C�}�[��1�ȏ�@���@�ő�l�ȉ��Ȃ猸�����o
	float Shake = 0.0f;
	if (m_HP_Break_Timer > 0 && m_HP_Break_Timer < m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer++;
		HP_Gauge_Now_Value = float(m_HP) + float(m_Before_HP - m_HP) * (1.0f - (float(m_HP_Break_Timer) / float(m_HP_GAUGE_BREAK_TIME)));
		Shake = float(m_HP_Break_Timer % 5);
	}
	// �������o�I���
	if (m_HP_Break_Timer == m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer = 0;
	}
	//if(m_HP < m_Before_HP)
	//m_HP_GAUGE_BREAK_TIME

	// ���݂�HP����
	float Now_HP_Rate = float(HP_Gauge_Now_Value) / float(m_MaxHP);
	// ���݂�HP����2
	float Now_HP_Rate2 = float(m_HP) / float(m_MaxHP);
	// HP�Q�[�W�̒���
	float Gauge_Max_Width = 357.0f - 22.0f;
	// ���݂�HP�Q�[�W�̒���
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

	// HP�̐��l�`��
	DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(m_MaxHP, 0, false), Vec2(333.0f + 5.0f, 489.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	if (m_MaxHP > 9) {// 2���ڂ̕`��
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(m_MaxHP, 1, false), Vec2(303.0f + 5.0f, 478.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}

	DrawFunc2D::DrawGraph(Vec2(252.0f - 7.0f, 461.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), m_NumberTex[10]);

	if (m_MaxHP > 99) {// 3���ڂ̕`��
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(m_MaxHP, 2, false), Vec2(270.0f + 5.0f, 468.0f - 2.0f) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}
	

	// 1���ڂ̕`��
	DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(int(HP_Gauge_Now_Value), 0, false), Vec2(222.0f - 5.0f, 447.0f - 2.0f - Shake) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	if (int(HP_Gauge_Now_Value) > 9) {// 2���ڂ̕`��
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(int(HP_Gauge_Now_Value), 1, false), Vec2(192.0f - 5.0f, 436.0f - 2.0f - Shake) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}
	if (int(HP_Gauge_Now_Value) > 99) {// 3���ڂ̕`��
		DrawFunc2D::DrawNumber2D(KuroEngine::GetSpecifiedDigitNum(int(HP_Gauge_Now_Value), 2, false), Vec2(162.0f - 5.0f, 426.0f - 2.0f - Shake) + ScreenShakeManager::Instance()->GetOffset(), &m_NumberTex.front());
	}

	// �A���e�B���b�g�Q�[�W�̕`��
	// ���݂̃Q�[�W����
	float Now_ULT_Rate = float(GetUltPoint()) / float(Max_UltimatePoint);
	// �Q�[�W�̍���
	float Gauge_ULT_Max_Width = 129.0f - 14.0f;
	// ���݂̃Q�[�W�̍���
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

	// �`��ʒu�̕ۑ�
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
	// HP�����Ă������o�p
	m_Before_HP = m_HP;
	m_HP_Break_Timer = 1;
	// �_���[�W
	m_HP -= value;
	if (m_HP < 0) {
		m_HP = 0;
	}

	// �^�C�}�[
	m_Damage_Timer = int(40.0f * RefreshRate::RefreshRate_Mag);

	// �`���[�g���A���Ȃ玀�ȂȂ�
	if (ExistUnits::Instance()->m_StageName == "Tutorial") {
		if (m_HP <= 0) {
			m_HP = 1;
		}
	}
}

void Player::TurnEnd_BeforeTurnChange()
{
	m_IsEndTurnFunc = true;
	// �����Ƀ{�[�i�X�A�^�b�N�Ƃ�����
	// �}�b�v�����Z�b�g
	m_DoBonus = true;
	ExistUnits::Instance()->m_IsEndBonusCount = false;
	ExistUnits::Instance()->m_StageManager->SetBouns();
	OperationConfig::Instance()->SetInGameOperationActive(false);
	// �{�[�i�X
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
	//��b�T�C�Y
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
