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

#include "../Effect/ScreenShakeManager.h"

void BattleTurnMgr::TurnEndButtonUpdate()
{
	using namespace KuroEngine;

	// �v���C���[�^�[���̂�
	if (TurnNum == 0 && UnitList[0]->IsAlive() && AliveEnemys()) {
		// �^�[���G���h�{�^���������ꂽ(����)
		if (!m_Selected_TurnEnd && !m_Checked_TurnEnd ) {
			if (OperationConfig::Instance()->GetOperationInput(OperationConfig::END_TURN, OperationConfig::ON_TRIGGER)) {
				m_Selected_TurnEnd = true;
			}
		}
		// 2��ڈȍ~
		else if (m_Selected_TurnEnd && !m_Checked_TurnEnd) {
			// �^�[���G���h�{�^���������ꂽ
			if (OperationConfig::Instance()->GetOperationInput(OperationConfig::END_TURN, OperationConfig::ON_TRIGGER)) {
				m_Checked_TurnEnd = true;
				m_Moving_Flag = true;
				GetUnitPtr<Player>(UnitList[0])->TurnEndTrigger();
			}
			// ����ȊO�̃{�^���������ꂽ
			else if (OperationConfig::Instance()->CheckAllOperationInputTrigger()) {
				m_Selected_TurnEnd = false;
			}
		}
	}

	// �ړ��t���O�������Ă鎞(�^�[���G���h�m�莞)
	if (m_Moving_Flag) {
		m_Moving_Timer < m_Moving_Timer_Max ? m_Moving_Timer++ : 0;
	}
	else {
		m_Moving_Timer > 0 ? m_Moving_Timer-- : m_Moving_Timer = 0;
	}

	// 1��ڂ̓��͂�����ҋ@��Ԃ��p�����Ă��鎞
	if (m_Selected_TurnEnd) {
		// �^�C�}�[�̍X�V
		m_Selected_TurnEnd_Timer++;
		m_Scaling_Timer < m_Scaling_Timer_Max ? m_Scaling_Timer++ : 0;
	}
	// �ҋ@��Ԃł͂Ȃ��Ƃ�
	else {
		// �^�C�}�[�̍X�V
		m_Selected_TurnEnd_Timer > 0 ? m_Selected_TurnEnd_Timer-- : m_Selected_TurnEnd_Timer = 0;
		m_Scaling_Timer > 0 ? m_Scaling_Timer-- : m_Scaling_Timer = 0;
	}
}

void BattleTurnMgr::TurnEndButtonDraw()
{
	using namespace KuroEngine;

	// SURE�傫��
	Vec2 SureTexSize_Int = m_TurnEnd_SelectedTex->GetGraphSize();
	Vec2 SureTexSize = Vec2(float(SureTexSize_Int.x), float(SureTexSize_Int.y));
	Vec2 SureRB = Vec2(930.0f, 583.0f);

	// �X�P�[��
	float Scale_Value = m_Scaling_Timer / m_Scaling_Timer_Max;
	Vec2 Scale_Size = SureTexSize * (Scale_Value > 1.0f ? 1.0f : Scale_Value);

	// �ʒu
	float Moving_Pars = m_Moving_Timer / m_Moving_Timer_Max;
	Vec2 Moving_Pos = Vec2(0.0f, 400.0f - ResultEasing(Moving_Pars) * 400.0f);

	// 1��ڂ̓��͂�����ҋ@��Ԃ��p�����Ă��鎞
	if (m_Selected_TurnEnd) {
		DrawFunc2D::DrawExtendGraph2D(SureRB - Scale_Size + Moving_Pos + ScreenShakeManager::Instance()->GetOffset()
			, SureRB + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_SelectedTex);
	}
	// �ҋ@��Ԃł͂Ȃ��Ƃ�
	else {
		DrawFunc2D::DrawExtendGraph2D(SureRB - Scale_Size + Moving_Pos + ScreenShakeManager::Instance()->GetOffset()
			, SureRB + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_SelectedTex);
	}

	Vec2 EndButton_Size_Int = m_TurnEndTex->GetGraphSize();
	Vec2 EndButton_Size = Vec2(float(EndButton_Size_Int.x), float(EndButton_Size_Int.y));
	Vec2 EndButton_LT = Vec2(893.0f, 559.0f);
	Vec2 EndButton_Center = EndButton_LT + EndButton_Size / 2.0f;
	DrawFunc2D::DrawExtendGraph2D(EndButton_Center - EndButton_Size / 2.0f + Moving_Pos + ScreenShakeManager::Instance()->GetOffset()
		, EndButton_Center + EndButton_Size / 2.0f + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEndTex);

	if (OperationConfig::Instance()->GetLatestDevice() == OperationConfig::Instance()->KEY_BOARD_MOUSE) {
		Vec2 EnterButton_Size_Int = m_TurnEnd_EnterTex->GetGraphSize();
		Vec2 EnterButton_Size = Vec2(float(EnterButton_Size_Int.x), float(EnterButton_Size_Int.y));
		Vec2 Height_ = Vec2(0.0f, EnterButton_Size.y / 2.0f - 8.0f);

		DrawFunc2D::DrawExtendGraph2D(EndButton_Center - EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(),
			EndButton_Center + EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_EnterTex);
	}
	else {
		Vec2 EnterButton_Size_Int = m_TurnEnd_Crtl_EnterTex->GetGraphSize();
		Vec2 EnterButton_Size = Vec2(float(EnterButton_Size_Int.x), float(EnterButton_Size_Int.y));
		Vec2 Height_ = Vec2(0.0f, EnterButton_Size.y / 2.0f - 8.0f);

		DrawFunc2D::DrawExtendGraph2D(EndButton_Center - EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(),
			EndButton_Center + EnterButton_Size / 2.0f + Height_ + Moving_Pos + ScreenShakeManager::Instance()->GetOffset(), m_TurnEnd_Crtl_EnterTex);
	}
}

float BattleTurnMgr::ResultEasing(float time)
{
	float t = time;
	if (t > 1.0f) {
		t = 1.0f;
	}
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	float Ret = t < 0.5f
		? (powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	return 1.0f - Ret;
}

void BattleTurnMgr::OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	m_Total_Turn_Count = 1;
	TurnNum = 0;
	TurnFrameTime = 0;
	NextGameTimer = 0;
	ExistUnits::Instance()->m_NowTarget = 0;
	m_IsDefeat = false;

	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_CutInTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "player_turn.png");

	// �^�[���G���h
	m_Selected_TurnEnd = false;
	m_Checked_TurnEnd = false;
	m_Selected_TurnEnd_Timer = 0;
	m_Scaling_Timer = 0;
	m_Scaling_Timer_Max = 20.0f * RefreshRate::RefreshRate_Mag;
	m_Moving_Flag = false;
	m_Moving_Timer = 0;
	m_Moving_Timer_Max = 80.0f * RefreshRate::RefreshRate_Mag;

	m_TurnEndTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "turn_end.png");
	m_TurnEnd_EnterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/done_key.png");
	m_TurnEnd_Crtl_EnterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/clear/done_controller.png");
	m_TurnEnd_SelectedTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "turn_end_sure.png");

	// ���e�B�N��
	Reticle::Instance()->SetBattleTurnManager(this);

	// �J�b�g�C���X�^�[�g
	CutInMgr::Instance()->StartCutIn(CutInType::PLAYER_TURN);
}

void BattleTurnMgr::SetUnits(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys)
{
	UnitList.clear();
	UnitList.emplace_back(Player);
	for (auto& en : Enemys) {
		UnitList.emplace_back(en);
	}
	m_Whole_Turn_Count = 0;
	m_Total_Turn_Count++;
	TurnNum = 0;
	TurnFrameTime = 0;
	NextGameTimer = 0;
	// �^�[���G���h
	m_Selected_TurnEnd = false;
	m_Checked_TurnEnd = false;
	m_Moving_Flag = false;
	m_Scaling_Timer_Max = 20.0f * RefreshRate::RefreshRate_Mag;

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

	TurnEndButtonUpdate();

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

	// �^�[���G���h�{�^��
	TurnEndButtonDraw();

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
				if (TurnNum == 0) {
					m_Moving_Flag = false;
				}
				m_Checked_TurnEnd = false;
				m_Selected_TurnEnd = false;
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
