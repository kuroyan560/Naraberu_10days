#include "UnitBase.h"

void UnitBase::StartTurn()
{
	// �^�[���X�^�[�g
	m_isMyTurn = true;
	// �^�C�}�[���Z�b�g
	m_Frame_Timer = 0;
	// 
	m_IsEndTurnFunc = false;

	m_NextTurn = false;
}

void UnitBase::EndTurn()
{
	// �^�[���I��
	m_isMyTurn = false;
	// �����^�[���ؑ�
	m_NextTurn = true;
}

void UnitBase::End()
{
	// �^�[���؂�ւ��t���O������
	m_NextTurn = false;
}
