#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../../src/engine/Common/Singleton.h"

#include "Debug_Attack_01.h"
#include "Debug_Attack_02.h"
#include "Debug_Heal_01.h"
#include "Debug_Jamming_01.h"

class UnitBase;

namespace EnemyActions {

	template <class T>
	static std::shared_ptr<T> GetPtr(std::shared_ptr<E_Ac_Base> ptr) { return std::dynamic_pointer_cast<T>(ptr); }

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

	class EnemyActionMgr : public KuroEngine::DesignPattern::Singleton<EnemyActionMgr>
	{
		friend class KuroEngine::DesignPattern::Singleton<EnemyActionMgr>;
	private:
		std::vector<std::shared_ptr<E_Ac_Base>> m_List;

		void Initialize();

	private:
		std::string m_Now_Action_Name;
		UnitBase* m_Initiator;
		std::vector<UnitBase*>m_Targets;

		bool m_NowAction;
	public:

		EnemyActionMgr() {
			{
				using enum EnemyActionList;
				for (int i = 0; i < int(ACTION_MAX); i++) {
					m_List.emplace_back(nullptr);
				}
				m_List[size_t(DEBUG_ATTACK_01)] = std::make_shared<Debug_Attack_01>();
				m_List[size_t(DEBUG_ATTACK_02)] = std::make_shared<Debug_Attack_02>();
				m_List[size_t(DEBUG_HEAL_01)] = std::make_shared<Debug_Heal_01>();
				m_List[size_t(DEBUG_JAMMING_01)] = std::make_shared<Debug_Jamming_01>();

				m_NowAction = false;
			}
		}

		void StartAction(std::string Action_Name);

		template<class... A>
		void SetUnits(UnitBase* Initiator, A... Targets) {
			m_Initiator = Initiator;
			m_Targets.clear();
			for (UnitBase* Target : std::initializer_list<UnitBase*>{ Targets... }) {
				m_Targets.emplace_back(Target);
			}
		}

		void Update();
		void Draw();

		bool GetEnd();
	};

	// -----------------------------
	// �s���ɕK�v�ȕ]���l
	// �E�S�Ẵ��j�b�g��HP
	// �E�Ֆʂ̏��
	// �E�����̃X�e�[�^�X
	// �E�v���C���[�̃X�e�[�^�X
	// �E�v���C���[�̏��

	// ----------------------------
	// �s�������ɕK�v�ȕϐ�
	// �E�s���^�C�v
	// �E�s�����ʁi�_���[�W(�񕜊܂�)�E��Ԉُ�E�Ֆʕω��ʒu�j
	// �E���A�N�V�����֘A
	// �@�E�A�C�R���̃V�F�C�N
	// �@�E�J�b�g�C���̗L��
}