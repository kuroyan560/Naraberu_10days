#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../../src/engine/Common/Singleton.h"

#include "Debug_Attack_01.h"

class UnitBase;

namespace EnemyActions {

	template <class T>
	static std::shared_ptr<T> GetPtr(std::shared_ptr<E_Ac_Base> ptr) { return std::dynamic_pointer_cast<T>(ptr); }

	static std::shared_ptr<E_Ac_Base> SetActionOnName(std::string Action_Name) {
		/*std::shared_ptr<E_Ac_Base> Data;
		if (Action_Name == "Attack_01") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else if (Action_Name == "Attack_02") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else if (Action_Name == "Heal_01") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else if (Action_Name == "Jamming_01") {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}
		else {
			std::shared_ptr<E_Ac_Base> Data = std::make_shared<Debug_Attack_01>();
		}*/
		return std::make_shared<Debug_Attack_01>();
	}

	enum class EnemyActionList {
		DEBUG_ATTACK_01,
		ACTION_MAX
	};
	static std::map<std::string, EnemyActionList> m_List_Pair = {
		{"Attack_01", EnemyActionList::DEBUG_ATTACK_01 },
		{"Attack_02", EnemyActionList::DEBUG_ATTACK_01 }
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

				m_NowAction = false;
			}
		}

		void StartAction(std::string Action_Name);

		template<class... A>
		void SetUnits(UnitBase* Initiator, A... Targets) {
			m_Initiator = Initiator;
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