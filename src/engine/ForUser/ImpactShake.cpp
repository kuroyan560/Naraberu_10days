#include "ImpactShake.h"
#include"KuroEngine.h"

void KuroEngine::ImpactShake::Update(float arg_timeScale)
{
	if (!m_active)return;

	//�U����
	if (!m_activeTimer.UpdateTimer(arg_timeScale))
	{
		//�U���X�p���v��
		if (m_spanTimer.UpdateTimer(arg_timeScale))
		{
			//���Ԃ��o�قǐU�����キ�Ȃ��Ă���
			float timeRate = m_activeTimer.GetTimeRate();
			float shakePowerScale = KuroEngine::Math::Ease(Out, Circ, timeRate, 1.0f, 0.0f);

			//�U���I�t�Z�b�g�l�����߂�
			m_offset.x = GetRand(m_powerMin, m_powerMax)
				* shakePowerScale * GetRandPlusMinus();
			m_offset.y = GetRand(m_powerMin, m_powerMax)
				* shakePowerScale * GetRandPlusMinus();
			m_offset.z = GetRand(m_powerMin, m_powerMax)
				* shakePowerScale * GetRandPlusMinus();

			m_spanTimer.Reset();
		}
	}
	else
	{
		//�U���I���
		m_offset = { 0,0,0 };
		m_active = false;
	}
}

void KuroEngine::ImpactShake::Shake(float arg_time, float arg_span, float arg_powerMin, float arg_powerMax)
{
	//�A�N�e�B�u��Ԃ�
	m_active = true;

	//�e��U���p�����[�^�Z�b�g
	m_activeTimer.Reset(arg_time);
	m_spanTimer.Reset(arg_span);
	m_powerMin = arg_powerMin;
	m_powerMax = arg_powerMax;

	//�U���I�t�Z�b�g�����l�v�Z
	m_offset.x = GetRand(arg_powerMin, arg_powerMax) * GetRandPlusMinus();
	m_offset.y = GetRand(arg_powerMin, arg_powerMax) * GetRandPlusMinus();
	m_offset.z = GetRand(arg_powerMin, arg_powerMax) * GetRandPlusMinus();
}
