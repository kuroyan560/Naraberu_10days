#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <forward_list>

#include "../../src/engine/Common/Singleton.h"

#include "Player_Attack_01.h"

class UnitBase;

namespace PlayerSkills {

	template <class T>
	static std::shared_ptr<T> GetPtr(std::shared_ptr<P_Ac_Base> ptr) { return std::dynamic_pointer_cast<T>(ptr); }

	enum class PlayerSkillList {
		PLAYER_ATTACK_01,
		ACTION_MAX
	};
	static std::map<std::string, PlayerSkillList> m_List_Pair = {
		{"Attack_01", PlayerSkillList::PLAYER_ATTACK_01 }
	};

	class PlayerSkillMgr : public KuroEngine::DesignPattern::Singleton<PlayerSkillMgr>
	{
		friend class KuroEngine::DesignPattern::Singleton<PlayerSkillMgr>;
	private:
		std::forward_list<std::shared_ptr<P_Ac_Base>> m_List;

		void Initialize();

	private:
		std::string m_Now_Action_Name;
		UnitBase* m_Initiator;
		//std::vector<UnitBase*>m_Targets;

		bool m_NowAction;
	public:

		PlayerSkillMgr() {
			{
				using enum PlayerSkillList;

				m_NowAction = false;
			}
		}

		template<class... A>
		void StartAction(std::string Action_Name, UnitBase* Initiator, A... Targets)
		{
			if (Action_Name == "Attack_01") {
				m_List.emplace_front(std::make_shared<Player_Attack_01>());
				GetPtr<Player_Attack_01>(m_List.front())->Param_Set(50, 3);
				m_Initiator = Initiator;
				std::vector<UnitBase*>m_Targets;
				m_Targets.clear();
				for (UnitBase* Target : std::initializer_list<UnitBase*>{ Targets... }) {
					m_Targets.emplace_back(Target);
				}
				GetPtr<Player_Attack_01>(m_List.front())->Need_Object_Set(Initiator, m_Targets[0]);
			}
		}

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
}