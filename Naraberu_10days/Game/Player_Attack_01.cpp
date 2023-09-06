#include "Player_Attack_01.h"
#include "Player.h"
#include "Enemy.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

void Player_Attack_01::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Attack_01::Action_Start()
{
	// ��ꏉ����
	P_Ac_Base::Action_Start();
}

void Player_Attack_01::Action_Update()
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
		// 
		if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
			exit(1);
		}
		// 1�Ԗڂ̃^�[�Q�b�g�ɍU������
		m_Targets[0]->Damage(m_Damage);
	}
}

void Player_Attack_01::Action_Draw()
{
	// �I���ς݂̏ꍇ�͕`�悵�Ȃ�
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// ���Ŕ���`��
	KuroEngine::Vec2 LT = GetUnitPtr_nama<Enemy*>(m_Targets[0])->Get_Left_Top();
	KuroEngine::Vec2 RB = GetUnitPtr_nama<Enemy*>(m_Targets[0])->Get_Right_Bottom();
	DrawFunc2D::DrawBox2D(LT, RB, Color(255, 0, 0, 255), true);
}

void Player_Attack_01::Action_End()
{
	// �I���t���O���グ��
	End();
}