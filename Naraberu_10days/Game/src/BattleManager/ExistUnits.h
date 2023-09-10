#pragma once
#include "Common/Singleton.h"
#include <vector>
#include <array>
#include <string>

class UnitBase;
class PanelManager;

class ExistUnits : public KuroEngine::DesignPattern::Singleton<ExistUnits>
{
	friend class KuroEngine::DesignPattern::Singleton<ExistUnits>;
public:
	UnitBase* m_pPlayer = nullptr;
	std::vector<UnitBase*>m_Enemys;
	std::vector<std::vector<int>>* m_MapChipData;
	PanelManager* m_StageManager = nullptr;

	int m_NowTurn;
	int m_NowTarget;
	int m_NowBonusCount = 0;
	int m_MaxBonusCount = 0;
	// ボーナスカウント演出が終わったかどうか
	bool m_IsEndBonusCount = false;

	// 
	bool m_ChangeStageSelect = false;

	// ロードするステージ名(現在のステージ名)
	std::string m_StageName;

	ExistUnits() {
	}

	void Set(UnitBase* Player, UnitBase* Enemy_01 = nullptr, UnitBase* Enemy_02 = nullptr, UnitBase* Enemy_03 = nullptr) {
		m_pPlayer = Player;
		m_Enemys.clear();
		m_Enemys.emplace_back(Enemy_01);
		if (Enemy_02 != nullptr) {
			m_Enemys.emplace_back(Enemy_02);
		}
		if (Enemy_03 != nullptr) {
			m_Enemys.emplace_back(Enemy_03);
		}
	}

	void SetStageManagerPtr(PanelManager* ptr) {
		m_StageManager = ptr;
	}

	void SetMapChipPtr(std::vector<std::vector<int>>* MapChipData) {
		m_MapChipData = MapChipData;
	}

	void AllClear() {
		m_Enemys.clear();
	}

	void SetBonusCount(int _nowBonusCount) {
		m_NowBonusCount = _nowBonusCount;
		if (_nowBonusCount > m_MaxBonusCount) {
			m_MaxBonusCount = _nowBonusCount;
		}
	}

	std::vector<UnitBase*> GetAliveUnit();
	std::vector<int> GetAliveEnemyIndex();
};