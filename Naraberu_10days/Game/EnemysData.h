#pragma once
#include <string>
#include <vector>
#include <utility>

namespace EnemysData {

	enum class AI_TYPE {
		DEFAULT, CUSTOM
	};

	class EnemyData {
	public:
		// ���j�b�g��
		std::string m_Name;
		// ���ݑ̗�
		int m_HP;
		// AI�̃^�C�v
		AI_TYPE m_AI_Type;
		// �s�����X�g
		std::vector<std::string> ActionList;

		EnemyData() : m_Name("UnNamed"), m_HP(1), m_AI_Type(AI_TYPE::DEFAULT) {}

		template<class... A>
		EnemyData(std::string Name, int HP, AI_TYPE AI_Type, A... ActPatterns) :
			m_Name(Name), m_HP(HP), m_AI_Type(AI_Type) {

			for (std::string ActionName : std::initializer_list<std::string>{ ActPatterns... }) {
				ActionList.emplace_back(ActionName);
			}
		}
	};

	// �����ɓG��ǉ����Ă���
	static const EnemyData DebugEnemy_1 = EnemyData("Debug_1", 1, AI_TYPE::DEFAULT, "Attack_01", "Attack_02");
	static const EnemyData DebugEnemy_2 = EnemyData("Debug_2", 256, AI_TYPE::DEFAULT, "Attack02", "Heal_01", "Jamming_01");

}