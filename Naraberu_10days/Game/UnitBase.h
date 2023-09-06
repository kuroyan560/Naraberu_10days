#pragma once
#include <memory>
#include "RefreshRate.h"
class UnitBase
{
protected:
	// HP
	int m_HP;
	int m_MaxHP;

	// HP�Q�[�W�����鉉�o�p�^�C�}�[
	int m_HP_Break_Timer;
	// ��_��or�񕜑O��HP
	int m_Before_HP;
	// HP�Q�[�W�����Ă�����
	int m_HP_GAUGE_BREAK_TIME;

	// ���ݎ����̃^�[����
	bool m_isMyTurn;
	// ���̃^�[���Ɉڍs����t���O
	bool m_NextTurn;
	// ���^�[���̌o�߃t���[���^�C�}�[
	int m_Frame_Timer;
public:
	UnitBase(){
		m_HP = 200;
		m_MaxHP = 200;
		m_HP_Break_Timer = 0;
		m_Before_HP = 200;
		m_HP_GAUGE_BREAK_TIME = int(50.0f * RefreshRate::RefreshRate_Mag);
		m_isMyTurn = false;
		m_NextTurn = false;
		m_Frame_Timer = 0;
	};

	virtual void OnInitialize() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnAlwaysUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnImguiDebug() = 0;
	virtual void OnFinalize() = 0;

	void StartTurn();
	void EndTurn();

	void TimerUpdate() {
		m_Frame_Timer++;
	}

	void End();

	bool IsNextTurn() { return m_NextTurn; }

	void Damage(int value) {
		// HP�����Ă������o�p
		m_Before_HP = m_HP;
		m_HP_Break_Timer = 1;
		// �_���[�W
		m_HP -= value;
		if (m_HP < 0) {
			m_HP = 0;
		}
	}

	void Heal(int value) {
		// HP���񕜂��鉉�o�p
		m_Before_HP = m_HP;
		m_HP_Break_Timer = 1;
		// ��
		m_HP += value;
		if (m_HP > m_MaxHP) {
			m_HP = m_MaxHP;
		}
	}
};

/// <summary>
/// Unitbase*�� UnitBase ���p������ class�^<T> �ɃL���X�g����
/// </summary>
/// <typeparam name="T">�L���X�g����class��</typeparam>
/// <param name="Unit">�L���X�g������UnitBase*�I�u�W�F�N�g</param>
/// <returns></returns>
template <class T>
static std::shared_ptr<T> GetUnitPtr(std::shared_ptr<UnitBase> Unit) { return std::dynamic_pointer_cast<T>(Unit); }

template <class T>
static T GetUnitPtr_nama(UnitBase* Unit) { return dynamic_cast<T>(Unit); }

// �g�p��
// Unit.emplace_back(new Enemy);
// Enemy* EnemyPtr = GetUnitPtr<Enemy*>(Unit);