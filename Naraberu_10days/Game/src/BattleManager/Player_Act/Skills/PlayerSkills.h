#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <forward_list>

#include "../../src/engine/Common/Singleton.h"

#include "Player_Attack_01.h"
#include "Player_Attack_02.h"
#include "Player_Heal_01.h"
#include "Player_Bonus_Attack_01.h"
#include "Player_Ultimate_01.h"

#include "../../../RefreshRate.h"

class UnitBase;

namespace PlayerSkills {

	template <class T>
	static std::shared_ptr<T> GetPtr(std::shared_ptr<P_Ac_Base> ptr) { return std::dynamic_pointer_cast<T>(ptr); }

	enum class PlayerSkillList {
		PLAYER_ATTACK_01,
		PLAYER_ATTACK_02,
		PLAYER_HEAL_01,
		PLAYER_BONUS_01,
		PLAYER_ULTIMATE_01,
		ACTION_MAX
	};
	static std::map<std::string, PlayerSkillList> m_List_Pair = {
		{"Attack_01", PlayerSkillList::PLAYER_ATTACK_01 },
		{"Attack_02", PlayerSkillList::PLAYER_ATTACK_02 },
		{"Heal_01", PlayerSkillList::PLAYER_HEAL_01 },
		{"Bonus_01", PlayerSkillList::PLAYER_BONUS_01 },
		{"Ultimate_01", PlayerSkillList::PLAYER_ULTIMATE_01 }
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
				m_NowAction = false;
			}
		}

		template<class... A>
		void StartAction(std::string Action_Name, int Block_Count, UnitBase* Initiator, A... Targets)
		{
			if (Action_Name == "Attack_01") {
				m_List.emplace_front(std::make_shared<Player_Attack_01>());
				GetPtr<Player_Attack_01>(m_List.front())->Param_Set(Block_Count, int(60.0f * RefreshRate::RefreshRate_Mag));
				m_Initiator = Initiator;
				std::vector<UnitBase*>m_Targets;
				m_Targets.clear();
				for (UnitBase* Target : std::initializer_list<UnitBase*>{ Targets... }) {
					m_Targets.emplace_back(Target);
				}
				GetPtr<Player_Attack_01>(m_List.front())->Need_Object_Set(Initiator, m_Targets[0]);
			}
			if (Action_Name == "Attack_02") {
				m_List.emplace_front(std::make_shared<Player_Attack_02>());
				GetPtr<Player_Attack_02>(m_List.front())->Param_Set(Block_Count, int(60.0f * RefreshRate::RefreshRate_Mag));
				m_Initiator = Initiator;
				std::vector<UnitBase*>m_Targets;
				m_Targets.clear();
				for (UnitBase* Target : std::initializer_list<UnitBase*>{ Targets... }) {
					m_Targets.emplace_back(Target);
				}
				GetPtr<Player_Attack_02>(m_List.front())->Need_Object_Set(Initiator, ExistUnits::Instance()->GetAliveUnit());
			}
			if (Action_Name == "Heal_01") {
				m_List.emplace_front(std::make_shared<Player_Heal_01>());
				GetPtr<Player_Heal_01>(m_List.front())->Param_Set(Block_Count, int(60.0f * RefreshRate::RefreshRate_Mag));
				m_Initiator = Initiator;
				GetPtr<Player_Heal_01>(m_List.front())->Need_Object_Set(Initiator);
			}
			if (Action_Name == "Bonus_01") {
				m_List.emplace_front(std::make_shared<Player_Bonus_Attack_01>());
				GetPtr<Player_Bonus_Attack_01>(m_List.front())->Param_Set(Block_Count, int(60.0f * RefreshRate::RefreshRate_Mag));
				m_Initiator = Initiator;
				std::vector<UnitBase*>m_Targets;
				m_Targets.clear();
				for (UnitBase* Target : std::initializer_list<UnitBase*>{ Targets... }) {
					m_Targets.emplace_back(Target);
				}
				GetPtr<Player_Bonus_Attack_01>(m_List.front())->Need_Object_Set(Initiator, ExistUnits::Instance()->GetAliveUnit());
			}
			if (Action_Name == "Ultimate_01") {
				m_List.emplace_front(std::make_shared<Player_Ultimate_01>());
				GetPtr<Player_Ultimate_01>(m_List.front())->Param_Set(50, int(71.0f * RefreshRate::RefreshRate_Mag));
				m_Initiator = Initiator;
				GetPtr<Player_Ultimate_01>(m_List.front())->Need_Object_Set(Initiator, ExistUnits::Instance()->GetAliveUnit());
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

		void AllClear() {
			m_List.clear();
		}
	};
}