#pragma once
#include <vector>
#include <string>
#include "../../src/engine/Common/Singleton.h"
#include "../BattleManager/Enemy/EnemysData.h"

namespace Stages {
	struct StageData {
		std::string m_Stage_Name;
		int m_Stage_Wave_Count;
		std::vector<std::vector<EnemysData::EnemyData>> m_Stage_Enemy_Data;

		StageData() {
			m_Stage_Name = "UNKNOWN_NAME";
			m_Stage_Wave_Count = 0;
		}
		StageData(std::string Stage_Name) {
			m_Stage_Name = Stage_Name;
			m_Stage_Wave_Count = 0;
		}
		// ウェーブ追加
		void AddWave(EnemysData::EnemyData Enemy_1, EnemysData::EnemyData Enemy_2 = EnemysData::EnemyData(), EnemysData::EnemyData Enemy_3 = EnemysData::EnemyData()) {
			std::vector<EnemysData::EnemyData> Wave_Enemys;
			Wave_Enemys.emplace_back(Enemy_1);
			if (Enemy_2.m_Name != "NONE") {
				Wave_Enemys.emplace_back(Enemy_2);
			}
			if (Enemy_3.m_Name != "NONE") {
				Wave_Enemys.emplace_back(Enemy_3);
			}
			m_Stage_Enemy_Data.emplace_back(Wave_Enemys);
		}
		// Index 番目のウェーブの敵を取得
		std::vector<EnemysData::EnemyData> GetWaveEnemyIndex(int Index) {
			// サイズより大きい場合は落とす
			if (m_Stage_Enemy_Data.size() < Index) {
				exit(1);
			}
			return m_Stage_Enemy_Data[Index - 1];
		}
	};

	class StageList : public KuroEngine::DesignPattern::Singleton<StageList>
	{
		friend class KuroEngine::DesignPattern::Singleton<StageList>;
		std::vector<StageData> m_Data;

	public:
		StageData GetStage(std::string StageName) {
			for (auto& data : m_Data) {
				if (data.m_Stage_Name == StageName) {
					return data;
				}
			}
			return StageData();
		}

		StageList() {
			m_Data.clear();
			using namespace EnemysData;
			m_Data.emplace_back(StageData("Stage1"));
			m_Data.back().AddWave(DebugEnemy_1, DebugEnemy_1, DebugEnemy_1);
			m_Data.back().AddWave(DebugEnemy_2, DebugEnemy_2, DebugEnemy_2);
			m_Data.back().AddWave(DebugEnemy_3, DebugEnemy_3, DebugEnemy_3);
			m_Data.back().AddWave(DebugEnemy_Boss_1);
			m_Data.emplace_back(StageData("Stage2"));
			m_Data.back().AddWave(DebugEnemy_2, DebugEnemy_2, DebugEnemy_2);
		}
	};
}