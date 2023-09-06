#include "BattleTurnMgr.h"
#include"../../src/engine/FrameWork/ImguiApp.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "Player.h"
#include "Enemy.h"

#include "../../src/engine/FrameWork/UsersInput.h"
#include "PlayerSkills.h"
#include "ExistUnits.h"

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");
}

void BattleTurnMgr::OnUpdate()
{
	// �펞�X�V����
	for (auto& Unit : UnitList) {
		Unit->OnAlwaysUpdate();
	}

	if (UnitList[TurnNum]->IsNextTurn()) {
		// �^�[���I������
		UnitList[TurnNum]->End();

		// �^�[���؂�ւ��E�S�̃^�[�������Z
		TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0, m_Whole_Turn_Count++;

		// �^�[���J�n����
		UnitList[TurnNum]->StartTurn();
		NextTurnStart();

		if (TurnNum == 0) {
			CutInMgr::Instance()->StartCutIn(CutInType::PLAYER_TURN);
		}
		else if (TurnNum == 1) {
			CutInMgr::Instance()->StartCutIn(CutInType::ENEMY_TURN);
		}
	}
	// �J�b�g�C�����ł����
	if (CutInMgr::Instance()->NowCutIn()) {
		CutInMgr::Instance()->OnUpdate();
	}
	if (!CutInMgr::Instance()->NowCutIn()) {
		// FrameTime���Z
		TurnFrameTime++;
		// �^�[���X�V
		UnitList[TurnNum]->OnUpdate();
	}

	if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_T)) {
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
	}
	PlayerSkills::PlayerSkillMgr::Instance()->Update();
}

void BattleTurnMgr::OnDraw()
{
	for (int i = 1; i < UnitList.size(); i++) {
		int EnemyIndex = i - 1;
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[i]);

		// �v���C���[�^�[���Ȃ�Â����Ȃ�
		if (TurnNum == 0) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count);
		}
		// �G�^�[���̎������̃^�[���ӊO�Ȃ�Â�����
		else if (i != TurnNum) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), true, TurnFrameTime, m_Whole_Turn_Count);
		}
		else {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count);
		}
	}

	// �G�`��
	UnitList[TurnNum]->OnDraw();

	// �v���C���[�X�L���`��
	PlayerSkills::PlayerSkillMgr::Instance()->Draw();

	using namespace KuroEngine;
	// �J�b�g�C�����ł����
	if (CutInMgr::Instance()->NowCutIn()) {
		CutInMgr::Instance()->OnDraw();
	}
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
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[TurnNum]);
		ImGui::Text(En->GetEnemyData().m_Name.c_str());
		ImGui::Text("HP : %d", En->GetEnemyData().m_HP);
		for (auto& ActionName : En->GetEnemyData().ActionList) {
			ImGui::Text(ActionName.c_str());
		}
	}
	ImGui::End();
}

void BattleTurnMgr::NextTurnStart()
{
	TurnFrameTime = 0;
}
