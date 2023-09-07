#pragma once
#include <vector>
#include <string>

namespace StageList {
	struct StageData {
		std::string m_Stage_Name;
		int m_Stage_Wave_Count;
		std::vector<std::vector<std::string>> m_Stage_Enemy_Data;

		StageData() {
			m_Stage_Name = "UNKNOWN_NAME";
			m_Stage_Wave_Count = 0;
		}
		StageData(std::string Stage_Name) {
			m_Stage_Name = Stage_Name;
			m_Stage_Wave_Count = 0;
		}
		// �E�F�[�u�ǉ�
		void AddWave(std::string EnemyName_1, std::string EnemyName_2 = "NONE", std::string EnemyName_3 = "NONE") {
			std::vector<std::string> Wave_Enemys;
			Wave_Enemys.emplace_back(EnemyName_1);
			if (EnemyName_2 != "NONE") {
				Wave_Enemys.emplace_back(EnemyName_2);
			}
			if (EnemyName_3 != "NONE") {
				Wave_Enemys.emplace_back(EnemyName_3);
			}
			m_Stage_Enemy_Data.emplace_back(Wave_Enemys);
		}
		// Index �Ԗڂ̃E�F�[�u�̓G�����擾
		std::vector<std::string> GetWaveEnemyIndex(int Index) {
			// �T�C�Y���傫���ꍇ�͗��Ƃ�
			if (m_Stage_Enemy_Data.size() < Index) {
				exit(1);
			}
			return m_Stage_Enemy_Data[Index - 1];
		}
	};

	//std::vector<StageData> StageList
}