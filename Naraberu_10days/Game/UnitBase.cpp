#include "UnitBase.h"

void UnitBase::StartTurn()
{
	// �^�[���X�^�[�g
	m_isMyTurn = true;
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
