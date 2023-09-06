#pragma once
#include <string>
#include <array>
#include <map>
#include "RefreshRate.h"

namespace En_Ac_Param {
	enum class EnemyActionList {
		DEBUG_ATTACK_01,
		DEBUG_ATTACK_02,
		DEBUG_HEAL_01,
		DEBUG_JAMMING_01,
		ACTION_MAX
	};

	static std::map<std::string, EnemyActionList> m_List_Pair = {
		{"Attack_01", EnemyActionList::DEBUG_ATTACK_01 },
		{"Attack_02", EnemyActionList::DEBUG_ATTACK_02 },
		{"Heal_01", EnemyActionList::DEBUG_HEAL_01 },
		{"Jamming_01", EnemyActionList::DEBUG_JAMMING_01 }
	};

	struct EnemyActionData {
		// �A�N�V������
		std::string m_Name;
		// �U���̃��[�g
		float m_AttackRate;
		// �񕜂̃��[�g
		float m_HealRate;
		// �A�N�V�����̍��v�t���[��
		int m_Duration;

		EnemyActionData() : m_Name("None_Name"), m_AttackRate(0.0f), m_HealRate(0.0f), m_Duration(1){}
		EnemyActionData(std::string Name, float AttackRate, float HealRate, int Duration) :
			m_Name(Name),
			m_AttackRate(AttackRate),
			m_HealRate(HealRate),
			m_Duration(int(float(Duration) * RefreshRate::RefreshRate_Mag)) {}
	};

	static std::array<EnemyActionData, size_t(EnemyActionList::ACTION_MAX)> Action_Original_Datas{
		EnemyActionData("Attack_01", 1.0f, 0.0f, 200),
		EnemyActionData("Attack_02", 1.5f, 0.0f, 200),
		EnemyActionData("Heal_01", 0.0f, 1.0f, 200),
		EnemyActionData("Jamming_01", 0.0f, 0.0f, 200)
	};
}