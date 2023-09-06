#pragma once
#include "P_Ac_Base.h"
#include <memory>
#include <vector>

class UnitBase;

class Player_Heal_01 : public P_Ac_Base
{

private:
	int m_Amount;

	// --------
	// Ž©•ª
	UnitBase* m_Initiator;

public:
	Player_Heal_01() {
		m_Amount = 0;
		m_Initiator = nullptr;
	};
	void Param_Set(int Amount, int Duration);

	template<class... A>
	void Need_Object_Set(UnitBase* Initiator) {
		m_Initiator = Initiator;
	}

public:
	void Action_Start()override;
	void Action_Update()override;
	void Action_Draw()override;
	void Action_End()override;
};

