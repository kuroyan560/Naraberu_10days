#include "Player_Attack_02.h"
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include "../../../Effect/ScreenShakeManager.h"
#include "../../../RefreshRate.h"

void Player_Attack_02::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Attack_02::Action_Start()
{
	// ��ꏉ����
	P_Ac_Base::Action_Start();
}

void Player_Attack_02::Action_Update()
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
		//m_Targets[0]->Damage(m_Damage);
		// �S�Ẵ^�[�Q�b�g�ɍU������
		for (auto& tgt : m_Targets) {
			//tgt->Damage(m_Damage * 2000);
			tgt->Damage(m_Damage);
		}
	}
}

void Player_Attack_02::Action_Draw()
{
	// �I���ς݂̏ꍇ�͕`�悵�Ȃ�
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// ���Ŕ���`��
	if (int(float(m_Timer) / (20.0f * RefreshRate::RefreshRate_Mag)) % 2 == 0) {
		for (auto& tgt : m_Targets) {
			GetUnitPtr_nama<Enemy*>(tgt)->Draw_Damage();
			/*KuroEngine::Vec2 LT = GetUnitPtr_nama<Enemy*>(tgt)->Get_Left_Top();
			KuroEngine::Vec2 RB = GetUnitPtr_nama<Enemy*>(tgt)->Get_Right_Bottom();
			DrawFunc2D::DrawBox2D(LT + ScreenShakeManager::Instance()->GetOffset()
				, RB + ScreenShakeManager::Instance()->GetOffset(), Color(255, 0, 0, 255), true);*/
		}
	}
}

void Player_Attack_02::Action_End()
{
	// �I���t���O���グ��
	End();
}
