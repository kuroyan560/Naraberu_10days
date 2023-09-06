#pragma once
#include "P_Ac_Base.h"
#include <memory>
#include <vector>

class UnitBase;

class Player_Bonus_Attack_01 :
    public P_Ac_Base
{
private:
	int m_Damage;
	int m_BonusCount;

	// --------
	// ����
	UnitBase* m_Initiator;
	std::vector<UnitBase*>m_Targets;

public:
	Player_Bonus_Attack_01() {
		m_Damage = 0;
		m_Initiator = nullptr;
	};
	void Param_Set(int Damage, int Duration);
	void Bonus_Count_Set(int BonusCount);

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

