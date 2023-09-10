#include "Player_Heal_01.h"
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include "../../../Effect/ScreenShakeManager.h"

#include"../../../SoundConfig.h"

void Player_Heal_01::Param_Set(int Amount, int Duration)
{
	m_Amount = Amount;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Heal_01::Action_Start()
{
	// ��ꏉ����
	P_Ac_Base::Action_Start();
}

void Player_Heal_01::Action_Update()
{
	// �I������
	if (GetEndTimer()) {
		Action_End();
		return;
	}

	// �^�C�}�[���Z
	m_Timer++;

	// �U���͈��̂�
	if (m_Timer == 1) {
		// �������񕜂���(����)
		m_Initiator->Heal_Par(m_Amount * 2);
		SoundConfig::Instance()->Play(SoundConfig::SE_HEAL);
	}
}

void Player_Heal_01::Action_Draw()
{
	// �I���ς݂̏ꍇ�͕`�悵�Ȃ�
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// ���Ŕ���`��
	/*KuroEngine::Vec2 LT = GetUnitPtr_nama<Player*>(m_Initiator)->Get_Left_Top();
	KuroEngine::Vec2 RB = GetUnitPtr_nama<Player*>(m_Initiator)->Get_Right_Bottom();
	DrawFunc2D::DrawBox2D(LT + ScreenShakeManager::Instance()->GetOffset()
		, RB + ScreenShakeManager::Instance()->GetOffset(), Color(0, 230, 30, 255), true);*/
}

void Player_Heal_01::Action_End()
{
	// �I���t���O���グ��
	End();
}
