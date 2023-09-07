#include "BattleTurnMgr.h"
#include "FrameWork/ImguiApp.h"
#include "FrameWork/WinApp.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

#include "FrameWork/UsersInput.h"
#include "Player_Act/Skills/PlayerSkills.h"
#include "ExistUnits.h"

#include"../OperationConfig.h"

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;
	ExistUnits::Instance()->m_NowTarget = 0;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");
}

void BattleTurnMgr::SetUnits(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;
	ExistUnits::Instance()->m_NowTarget = 0;
}

void BattleTurnMgr::OnUpdate()
{
	// �펞�X�V����
	for (auto& Unit : UnitList) {
		Unit->OnAlwaysUpdate();
	}

	// �G�̐�������
	bool EnemyAlive = false;

	// �v���C���[�^�[���G��������
	// �����Ă�G�����邩�m�F
	for (int i = 1; i < UnitList.size(); i++) {
		if (UnitList[i]->IsAlive()) {
			// �����Ă�G������΃t���O���グ��
			EnemyAlive = true;
		}
	}

	// �v���C���[������ł���
	if (!UnitList[TurnNum]->IsAlive()) {
		// �o�g���I������(�s�k)
		int a = 0;
	}

	// �����Ă�G�����Ȃ�
	if (!EnemyAlive) {
		// �o�g���I������(����)
		int a = 0;
	}

	// �ʏ�̍X�V����
	if (EnemyAlive && UnitList[TurnNum]->IsAlive()) {
		Update_Battle();
	}
	

	/*if (KuroEngine::UsersInput::Instance()->KeyOnTrigger(DIK_T)) {
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
	}*/

	// �^�[�Q�b�g


	// �X�L���̍X�V
	PlayerSkills::PlayerSkillMgr::Instance()->Update();
}

void BattleTurnMgr::OnDraw()
{
	for (int i = 1; i < UnitList.size(); i++) {
		int EnemyIndex = i - 1;
		std::shared_ptr<Enemy> En = GetUnitPtr<Enemy>(UnitList[i]);

		// �v���C���[�^�[���Ȃ�Â����Ȃ�
		if (TurnNum == 0) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
		// �G�^�[���̎������̃^�[���ӊO�Ȃ�Â�����
		else if (i != TurnNum) {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), true, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
		}
		else {
			En->Draw(EnemyIndex, TurnNum, int(UnitList.size()), false, TurnFrameTime, m_Whole_Turn_Count, ExistUnits::Instance()->m_NowTarget);
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

void BattleTurnMgr::Update_Battle()
{
	if (UnitList[TurnNum]->IsNextTurn()) {
		// �^�[���I������
		UnitList[TurnNum]->End();

		// �^�[���؂�ւ��E�S�̃^�[�������Z
		// �����Ă郆�j�b�g�܂Ŕ�΂�
		while (1) {
			TurnNum < UnitList.size() - 1 ? TurnNum++ : TurnNum = 0, m_Whole_Turn_Count++;
			// �����Ă郆�j�b�g�̃^�[��
			if (UnitList[TurnNum]->IsAlive()) {
				UnitList[TurnNum]->StartTurn();
				NextTurnStart();
				break;
			}
		}

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

	ExistUnits::Instance()->m_NowTurn = TurnNum;
	// ���b�N�I��
	if (OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
		// �����Ă��郆�j�b�g�܂�
		int iaaa = ExistUnits::Instance()->m_NowTarget;
		bool ChangeTargetSuccess = false;
		for (int i = ExistUnits::Instance()->m_NowTarget; i > 0; i--) {
			if (UnitList[i]->IsAlive()) {
				ExistUnits::Instance()->m_NowTarget = i - 1;
				ChangeTargetSuccess = true;
				break;
			}
		}
		// �^�[�Q�b�g��ύX�ł��Ȃ�����
		if (!ChangeTargetSuccess) {

		}
	}
	if (OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
		// �����Ă��郆�j�b�g�܂�
		int iaaa = ExistUnits::Instance()->m_NowTarget;
		bool ChangeTargetSuccess = false;
		for (int i = ExistUnits::Instance()->m_NowTarget + 1; i < UnitList.size() - 1; i++) {
			if (UnitList[i + 1]->IsAlive()) {
				ExistUnits::Instance()->m_NowTarget = i;
				ChangeTargetSuccess = true;
				break;
			}
		}
		// �^�[�Q�b�g��ύX�ł��Ȃ�����
		if (!ChangeTargetSuccess) {

		}
	}
	// �^�[�Q�b�g���̓G������ł���ꍇ
	if (!UnitList[ExistUnits::Instance()->m_NowTarget + 1]->IsAlive()) {
		// ��ԏ�̐����Ă���G��_��
		for (int i = 1; i < UnitList.size(); i++) {
			if (UnitList[i]->IsAlive()) {
				ExistUnits::Instance()->m_NowTarget = i - 1;
				break;
			}
		}
	}
}