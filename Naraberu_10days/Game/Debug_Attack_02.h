#pragma once
#include "E_Ac_Base.h"
#include <memory>

class UnitBase;

class Debug_Attack_02 : public E_Ac_Base
{

private:
	int m_Damage;

	// --------
	// Ž©•ª
	UnitBase* m_pUnit;
public:
	Debug_Attack_02() {
		m_Damage = 0;
		m_pUnit = nullptr;
	};
	void Param_Set(int Damage, int Duration);
	void Need_Object_Set(UnitBase* pUnit);

public:
	void Action_Start()override;
	void Action_Update()override;
	void Action_Draw()override;
	void Action_End()override;
};

