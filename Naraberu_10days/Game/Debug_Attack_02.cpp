#include "Debug_Attack_02.h"
#include "Enemy.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

void Debug_Attack_02::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}

void Debug_Attack_02::Action_Start()
{
	// ��ꏉ����
	E_Ac_Base::Action_Start();
}

void Debug_Attack_02::Action_Update()
{
	// �I������
	if (GetEndTimer()) {
		Action_End();
		return;
	}

	// �^�C�}�[���Z
	m_Timer++;

	// �U���͈��̂�
	if (m_Timer == 10) {
		// �Ώۂ�������Ȃ�
		if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
			exit(1);
		}
		// 1�Ԗڂ̃^�[�Q�b�g�ɍU������
		m_Targets[0]->Damage(m_Damage);
	}
}

void Debug_Attack_02::Action_Draw()
{
	// �I���ς݂̏ꍇ�͕`�悵�Ȃ�
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// ���Ŕ���`��
	//DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), Vec2(100.0f, 100.0f), Color(0, 0, 255, 255), true);
}

void Debug_Attack_02::Action_End()
{
	// �I���t���O���グ��
	End();
}
