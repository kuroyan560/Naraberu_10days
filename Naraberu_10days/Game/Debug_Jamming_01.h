#pragma once
#include "E_Ac_Base.h"
#include <memory>
#include <vector>

class UnitBase;

class Debug_Jamming_01 : public E_Ac_Base
{
private:
	// --------
	// ����
	UnitBase* m_Initiator;
	std::vector<std::vector<int>>* m_Mapchip;

public:
	Debug_Jamming_01() {
		m_Initiator = nullptr;
	};
	void Param_Set(int Damage, int Duration);
	//void Need_Object_Set(UnitBase* pUnit);

	template<class... A>
	void Need_Object_Set(UnitBase* Initiator, std::vector<std::vector<int>>* mapchip) {
		m_Initiator = Initiator;
		m_Mapchip = mapchip;
	}

public:
	void Action_Start()override;
	void Action_Update()override;
	void Action_Draw()override;
	void Action_End()override;
};