#pragma once
#include "P_Ac_Base.h"
#include <memory>
#include <vector>

class UnitBase;

class Player_Attack_02 : public P_Ac_Base
{

private:
	int m_Damage;

	// --------
	// ����
	UnitBase* m_Initiator;
	std::vector<UnitBase*>m_Targets;

public:
	Player_Attack_02() {
		m_Damage = 0;
		m_Initiator = nullptr;
	};
	void Param_Set(int Damage, int Duration);

	template<class... A>
	void Need_Object_Set(UnitBase* Initiator, std::vector<UnitBase*> Target) {
		m_Initiator = Initiator;

		m_Targets.clear();
		m_Targets = Target;
	}

public:
	void Action_Start()override;
	void Action_Update()override;
	void Action_Draw()override;
	void Action_End()override;
};

