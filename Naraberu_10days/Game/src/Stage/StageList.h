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

			m_Stage_Wave_Count++;
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
			m_Data.emplace_back(StageData("Tutorial"));
			m_Data.back().AddWave(Inu_Tutorial_1, Inu_Tutorial_1);
			m_Data.back().AddWave(Inu_Tutorial_3, Inu_Tutorial_3);

			m_Data.emplace_back(StageData("Stage1"));
			//m_Data.back().AddWave(Token_0, DebugEnemy_Boss_1);//
			m_Data.back().AddWave(Lizard_Blue_1, Lizard_Blue_1);
			m_Data.back().AddWave(Lizard_Blue_1, Inu_Blue_1);
			m_Data.back().AddWave(Zako_Blue_1, Inu_Yellow_1, Zako_Blue_1);
			m_Data.emplace_back(StageData("Stage2"));
			//m_Data.back().AddWave(Lizard_Blue_1, Lizard_Blue_1, Lizard_Blue_1);//
			m_Data.back().AddWave(Inu_Blue_1, Lizard_Blue_1, Inu_Blue_1);
			m_Data.back().AddWave(Inu_Blue_1, Lizard_Yellow_1, Inu_Blue_1);
			m_Data.back().AddWave(Zako_Blue_1, Lizard_Red_1, Zako_Blue_1);
			m_Data.emplace_back(StageData("Stage3"));
			m_Data.back().AddWave(Lizard_Yellow_1, Zako_Red_1, Inu_Blue_1);
			m_Data.back().AddWave(Lizard_Yellow_1, Zako_Red_1, Lizard_Yellow_1);
			m_Data.back().AddWave(Zako_Yellow_1, Lizard_Red_1, Zako_Yellow_1);
			m_Data.emplace_back(StageData("Stage4"));
			m_Data.back().AddWave(Token_0, DebugEnemy_Boss_1);
			m_Data.back().AddWave(Lizard_Red_1, Zako_Red_1, Lizard_Red_1);
			m_Data.back().AddWave(Inu_Red_1, Inu_Red_1, Lizard_Red_1);
			m_Data.back().AddWave(Token_0, DebugEnemy_Boss_1);


			m_Data.emplace_back(StageData("Challenge"));
			{
				int Level = 0;
				EnemyData ENdatas[10] = {
					Inu_Blue_1, Lizard_Blue_1, Zako_Blue_1,
					Inu_Yellow_1, Lizard_Yellow_1, Zako_Yellow_1,
					Inu_Red_1, Lizard_Red_1, Zako_Red_1,
					DebugEnemy_Boss_1
				};
				for (int i = 0; i < 8; i++) {
					for (int i = 0; i < 5; i++) {
						EnemyData data[3] = { Lizard_Blue_1 };

						int EnemyCount = 1;
						int type = 0;

						if (Level == 0) { // 0
							EnemyCount = 2;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(0, 2);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 1) { // 5
							EnemyCount = 3;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(0, 2);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 2) { // 10
							EnemyCount = 3;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(0, 5);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 3) { // 15
							EnemyCount = 3;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(3, 5);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 4) { // 20
							EnemyCount = 3;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(3, 8);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 5) { // 25
							EnemyCount = 3;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(6, 8);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 6) { // 30
							EnemyCount = 3;
							for (int cc = 0; cc < EnemyCount; cc++) {
								type = GetRand_(6, 8);
								data[cc] = ENdatas[type];
							}
						}
						if (Level == 7) { // 35
							EnemyCount = 4;
						}

						if (EnemyCount == 1) {
							m_Data.back().AddWave(data[0]);
						}
						else if (EnemyCount == 2) {
							m_Data.back().AddWave(data[0], data[1]);
						}
						else if (EnemyCount == 3) {
							m_Data.back().AddWave(data[0], data[1], data[2]);
						}
						else if (EnemyCount == 4) {
							m_Data.back().AddWave(Token_0, DebugEnemy_Boss_1);
						}
					}
					Level++;
				}
			}
		}
	};
}