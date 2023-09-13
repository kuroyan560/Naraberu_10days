#include "Debug_Jamming_01.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"
#include"../../../SoundConfig.h"
#include "../../ExistUnits.h"
#include "../../../Panel/PanelManager.h"

void Debug_Jamming_01::Action_Start()
{
	// ��ꏉ����
	E_Ac_Base::Action_Start();
}

void Debug_Jamming_01::Action_Update()
{
	// �I������
	if (GetEndTimer()) {
		Action_End();
		return;
	}

	// �^�C�}�[���Z
	m_Timer++;

	using namespace KuroEngine;

	// ���ז��v���Y����ݒu
	// �s���͈��̂�
	if (m_Timer == 10) {
		// �p�^�[��������
		int JamPattern = GetUnitPtr_nama<Enemy*>(m_Initiator)->m_Data.m_JammingPattern;
		// 
		// ��
		int Digit = GetRand(0, 4);
		int pattern = KuroEngine::GetSpecifiedDigitNum(JamPattern, Digit, false);

		// �����_������ 3��
		if (pattern == 0) {
			for (int i = 0; i < 3; i++) {
				Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
				ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			}
		}
		// �����_������ 3�� �v���C���[��10�_���[�W
		if (pattern == 1) {
			for (int i = 0; i < 3; i++) {
				Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
				ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			}
			ExistUnits::Instance()->m_pPlayer->Damage(10);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// �����_������ 3�� �v���C���[��20�_���[�W
		if (pattern == 2) {
			for (int i = 0; i < 3; i++) {
				Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
				ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			}
			ExistUnits::Instance()->m_pPlayer->Damage(20);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// 2 * 2 �𐶐�
		else if (pattern == 3) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
		}
		// 3 * 3 �ŏ\���̌����� �𐶐�
		else if (pattern == 4) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 8), KuroEngine::GetRand(0, 8));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 2));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 2));
		}
		// �\�� �𐶐�
		else if (pattern == 5) {
			Vec2<int> SetPos;
			// (1,1)���S�����s���m�肷��z�u�̓_��
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(1, 9), KuroEngine::GetRand(1, 9));
				if (SetPos == Vec2(1, 1) || SetPos == Vec2(8, 8) || SetPos == Vec2(1, 8) || SetPos == Vec2(8, 1)) {
					continue;
				}
				else {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(-1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, -1));
		}
		// �\�� �𐶐� �� �v���C���[��40�_���[�W
		else if (pattern == 6) {
			Vec2<int> SetPos;
			// (1,1)���S�����s���m�肷��z�u�̓_��
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(1, 9), KuroEngine::GetRand(1, 9));
				if (SetPos == Vec2(1, 1) || SetPos == Vec2(8, 8) || SetPos == Vec2(1, 8) || SetPos == Vec2(8, 1)) {
					continue;
				}
				else {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(-1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, -1));
			ExistUnits::Instance()->m_pPlayer->Damage(40);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// 2 * 2 �� 2�� ����
		else if (pattern == 7) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
			// �����ʒu�͔�����
			Vec2 Old = SetPos;
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
				if (Old != SetPos) {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
		}
		// 2 * 2 �� 2�� ���� �� �v���C���[��40�_���[�W
		else if (pattern == 8) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
			// �����ʒu�͔�����
			Vec2 Old = SetPos;
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
				if (Old != SetPos) {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
			ExistUnits::Instance()->m_pPlayer->Damage(40);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// 3 * 3 �ŏ\���̌����� �𐶐� �� �v���C���[��70�_���[�W
		else if (pattern == 9) {
		Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 8), KuroEngine::GetRand(0, 8));
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 2));
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 0));
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 2));
		ExistUnits::Instance()->m_pPlayer->Damage(70);
		SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}

		SoundConfig::Instance()->Play(SoundConfig::SE_PUT_OJAMA);
	}
}

void Debug_Jamming_01::Action_Draw()
{
	// �I���ς݂̏ꍇ�͕`�悵�Ȃ�
	if (GetEnd()) {
		return;
	}

	// ���Ŕ���`��
	//DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), Vec2(100.0f, 100.0f), Color(0, 255, 255, 255), true);
}

void Debug_Jamming_01::Action_End()
{
	// �I���t���O���グ��
	End();
}
