#include "BattleTurnMgr.h"
#include "FrameWork/ImguiApp.h"
#include "FrameWork/WinApp.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

#include "FrameWork/UsersInput.h"
#include "Player_Act/Skills/PlayerSkills.h"
#include "ExistUnits.h"
#include "Reticle/Reticle.h"
#include"../OperationConfig.h"

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	TurnNum = 0;
	TurnFrameTime = 0;
	NextGameTimer = 0;
	ExistUnits::Instance()->m_NowTarget = 0;
	m_IsDefeat = false;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");
	// ���e�B�N��
	Reticle::Instance()->SetBattleTurnManager(this);
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
	NextGameTimer = 0;
	ExistUnits::Instance()->m_NowTarget = 0;
	m_IsDefeat = false;
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
	if (!UnitList[0]->IsAlive()) {
		// �o�g���I������(�s�k)
		int a = 0;
		m_IsDefeat = true;
	}

	// �����Ă�G�����Ȃ�
	if (!EnemyAlive) {
		// �o�g���I������(����)
		Update_NextWave();
	}

	// �ʏ�̍X�V����
	if (EnemyAlive && UnitList[TurnNum]->IsAlive() && UnitList[0]->IsAlive()) {
		Update_Battle();
	}
	// ���b�N�I��
	Reticle::Instance()->Update();

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

void BattleTurnMgr::Update_NextWave()
{
	NextGameTimer++;
	if (NextGameTimer >= NextGameTimeFinish) {

	}
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
}

bool BattleTurnMgr::AliveEnemys()
{
	for (int i = 1; i < UnitList.size(); i++) {
		if (UnitList[i]->IsAlive()) {
			// �����Ă�G������΃t���O���グ��
			return true;
		}
	}
	return false;
}
