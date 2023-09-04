#include "BattleTurnMgr.h"
#include"../../src/engine/FrameWork/ImguiApp.h"
#include "Player.h"
#include "Enemy.h"

void BattleTurnMgr::OnInitialize(UnitBase* Player, std::vector<UnitBase*> Enemys)
{
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	TurnNum = 0;
}

void BattleTurnMgr::OnUpdate()
{
	if (UnitList[TurnNum]->IsNextTurn()) {
		// �^�[���I������
		UnitList[TurnNum]->End();

		// �^�[���؂�ւ�
		TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0;

		// �^�[���J�n����
		UnitList[TurnNum]->StartTurn();
	}
	// �^�[���X�V
	UnitList[TurnNum]->OnUpdate();
}

void BattleTurnMgr::OnImguiDebug()
{
	ImGui::Begin("GameScene");
	ImGui::Text("Naw Trun is %d", TurnNum);
	if (TurnNum == 0) {
		ImGui::Text("Type : Player");
	}
	else {
		ImGui::Text("Type : Enemy");
		Enemy* En = GetUnitPtr<Enemy*>(UnitList[TurnNum]);
		ImGui::Text(En->GetEnemyData().m_Name.c_str());
		ImGui::Text("HP : %d", En->GetEnemyData().m_HP);
		for (auto& ActionName : En->GetEnemyData().ActionList) {
			ImGui::Text(ActionName.c_str());
		}
	}
	ImGui::End();
}
