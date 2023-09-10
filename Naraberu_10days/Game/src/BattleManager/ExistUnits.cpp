#include "ExistUnits.h"
#include "UnitBase.h"

std::vector<UnitBase*> ExistUnits::GetAliveUnit()
{
	std::vector<UnitBase*> Result;
	for (auto& data : m_Enemys) {
		if (data->IsAlive()) {
			Result.emplace_back(data);
		}
	}
	return Result;
}

std::vector<int> ExistUnits::GetAliveEnemyIndex()
{
	std::vector<int> Result;
	for (int i = 0; i < m_Enemys.size(); i++) {
		if (m_Enemys[i]->IsAlive()) {
			Result.emplace_back(i);
		}
	}
	return Result;
}
