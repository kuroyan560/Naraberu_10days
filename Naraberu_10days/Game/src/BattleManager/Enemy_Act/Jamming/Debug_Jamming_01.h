#pragma once
#include <memory>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "../E_Ac_Base.h"
#include "../EnemyActionParametor.h"

class UnitBase;

class Debug_Jamming_01 : public E_Ac_Base
{
private:
	En_Ac_Param::EnemyActionData m_Data;

	// --------
	// Ž©•ª
	UnitBase* m_Initiator;
	std::vector<std::vector<int>>* m_Mapchip;

	int GetRand_(int Min, int Max)
	{
		static bool initialized = false;
		if (!initialized) {
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
			initialized = true;
		}
		int result = Min + std::rand() % (Max - Min + 1);
		return result;
	}
public:
	Debug_Jamming_01() {
		m_Initiator = nullptr;
		using namespace En_Ac_Param;
		m_Data = Action_Original_Datas[int(EnemyActionList::DEBUG_JAMMING_01)];
		m_Finish = m_Data.m_Duration;
	};
	void Ready() {
		m_Timer = 0;
	}
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
