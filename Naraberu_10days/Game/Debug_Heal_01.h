#pragma once
#include <memory>
#include <vector>

#include "E_Ac_Base.h"
#include "EnemyActionParametor.h"

class UnitBase;

class Debug_Heal_01 : public E_Ac_Base
{
private:
	En_Ac_Param::EnemyActionData m_Data;

	// --------
	// Ž©•ª
	UnitBase* m_Initiator;
	std::vector<UnitBase*>m_Targets;

public:
	Debug_Heal_01() {
		m_Initiator = nullptr;
		using namespace En_Ac_Param;
		m_Data = Action_Original_Datas[int(EnemyActionList::DEBUG_HEAL_01)];
		m_Finish = m_Data.m_Duration;
	}
	void Ready() {
		m_Timer = 0;
	}
	//void Need_Object_Set(UnitBase* pUnit);

	template<class... A>
	void Need_Object_Set(UnitBase* Initiator, UnitBase* Target) {
		m_Initiator = Initiator;

		m_Targets.clear();
		m_Targets.emplace_back(Target);
	}

public:
	void Action_Start()override;
	void Action_Update()override;
	void Action_Draw()override;
	void Action_End()override;
};

