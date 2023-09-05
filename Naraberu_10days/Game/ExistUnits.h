#pragma once
#include "../../src/engine/Common/Singleton.h"
#include <vector>

class UnitBase;

class ExistUnits : public KuroEngine::DesignPattern::Singleton<ExistUnits>
{
	friend class KuroEngine::DesignPattern::Singleton<ExistUnits>;
public:
	UnitBase* m_pPlayer = nullptr;
	std::vector<UnitBase*>m_Enemys;

	std::vector<std::vector<int>>* m_MapChipData;

	void Set(UnitBase* Player, UnitBase* Enemy_01 = nullptr, UnitBase* Enemy_02 = nullptr, UnitBase* Enemy_03 = nullptr) {
		m_pPlayer = Player;
		m_Enemys.clear();
		m_Enemys.emplace_back(Enemy_01);
		m_Enemys.emplace_back(Enemy_02);
		m_Enemys.emplace_back(Enemy_03);
	}

	void SetMapChipPtr(std::vector<std::vector<int>>* MapChipData) {
		m_MapChipData = MapChipData;
	}
};