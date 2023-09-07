#include "BattleScene.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

#include "../BattleManager/ExistUnits.h"

#include "../OperationConfig.h"

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/Enemy_Act/EnemyActList.h"

void BattleScene::OnInitialize()
{
	// ---- �w�i
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_SukasiTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "Sukasi.png");
	m_BackTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "battle_scene_frame.png");
	m_StageTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/info/stage.png");
	m_BattleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "/info/battle.png");
	D3D12App::Instance()->GenerateTextureBuffer(&m_NumberTex.front(), TexDir + "/info/stage_number.png", 12, Vec2(12, 1));
	D3D12App::Instance()->GenerateTextureBuffer(&m_NumberTex_Battle.front(), TexDir + "/info/battle_number.png", 11, Vec2(11, 1));
	m_Stage_End = false;
	m_Impossible_Put_Block_Effect_Time = int(50.0f * RefreshRate::RefreshRate_Mag);
	m_Impossible_Put_Block_Timer = m_Impossible_Put_Block_Effect_Time;
	m_End_Timer = 0;
	m_End_Timer_Finish = int(500.0f * RefreshRate::RefreshRate_Mag);

	Pl = std::make_shared<Player>();
	Pl->OnInitialize();
	Pl->StartTurn();

	// �X�e�[�W���Z�b�g
	SetStage(ExistUnits::Instance()->m_StageName);
	// �ŏ��̃E�F�[�u
	m_NowWave = 1;
	// �E�F�[�u�̓G���擾
	std::vector<EnemysData::EnemyData> EnemyData = m_NowStage.GetWaveEnemyIndex(m_NowWave);
	for (auto& data : EnemyData) {
		En.emplace_back(std::make_shared<Enemy>());
		GetUnitPtr<Enemy>(En.back())->SetEnemyData(data);
	}
	// �f�[�^���Z�b�g
	Mgr.OnInitialize(Pl, En);
	ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());

	stage.reset(new PanelManager());
	stage->Initialize();
	block.reset(new BlockManager());
	//�����w��
	block->SetCenter(stage->GetCenter());
	block->Initialize();


	ExistUnits::Instance()->SetMapChipPtr(stage->GetMapChipPtr());
	ExistUnits::Instance()->SetStageManagerPtr(stage.get());
}

void BattleScene::OnUpdate()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	// �E�F�[�u�I���E���E�F�[�u�X�^�[�g
	if (Mgr.ChangeNextWave()) {
		NextWave();
	}

	// �X�e�[�W�I��(�s�k)
	if (Mgr.GetDefeat()) {
		m_End_Timer++;
		if (m_End_Timer == m_End_Timer_Finish) {
			KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title");
		}
	}
	// �X�e�[�W�I��(�G�S��)
	else if (m_Stage_End) {
		//KuroEngine::AppearMessageBox("�X�e�[�W�I��", "�X�e�[�W�I��");
		m_End_Timer++;
		if (m_End_Timer == m_End_Timer_Finish) {
			KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title");
		}
	}

	//�Z�b�g�\�Ȃ�Z�b�g����
	if (ExistUnits::Instance()->m_NowTurn == 0 && m_Impossible_Put_Block_Timer == 0 && Mgr.AliveEnemys()) {
		PlayerTurn();
	}

	//���Z�b�g
	if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_I)) {
		stage->Reset();
	}

	// �ݒu�s���Ԃ̍X�V
	if (m_Impossible_Put_Block_Timer > 0) {
		m_Impossible_Put_Block_Timer--;
	}

	Mgr.OnUpdate();
	stage->Update();
	block->Update();

	//�Q�[���I�[�o�[�������̓N���A��������V�[����؂�ւ�����悤�ɂ���
	/*if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_RETURN)) {
		KuroEngine::KuroEngineDevice::Instance()->ChangeScene("title");
	}*/
}

void BattleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});

	// �w�i�F
	DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), Color(50, 49, 59, 255));
	// V�L�[�������Ă�Ԃ�����������`��
	if (UsersInput::Instance()->KeyInput(DIK_V)) {
		DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_SukasiTex);
	}
	// �w�i�`��
	DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), WinApp::Instance()->GetExpandWinSize(), m_BackTex);
	DrawFunc2D::DrawGraph(Vec2(881.0f, 19.0f), m_StageTex);
	DrawFunc2D::DrawGraph(Vec2(1064.0f, 13.0f), m_BattleTex);

	// �X�e�[�W���`��
	if (ExistUnits::Instance()->m_StageName == "Stage1") {
		DrawFunc2D::DrawNumber2D(1, Vec2(986.0f, 19.0f), &m_NumberTex.front());
		DrawFunc2D::DrawGraph(Vec2(1003.0f, 19.0f), m_NumberTex[10]);
		DrawFunc2D::DrawNumber2D(1, Vec2(1022.0f, 19.0f), &m_NumberTex.front());
	}
	else if (ExistUnits::Instance()->m_StageName == "Stage1") {
		DrawFunc2D::DrawNumber2D(1, Vec2(986.0f, 19.0f), &m_NumberTex.front());
		DrawFunc2D::DrawGraph(Vec2(1003.0f, 19.0f), m_NumberTex[10]);
		DrawFunc2D::DrawNumber2D(2, Vec2(1022.0f, 19.0f), &m_NumberTex.front());
	}
	else if (ExistUnits::Instance()->m_StageName == "Stage1") {
		DrawFunc2D::DrawNumber2D(1, Vec2(986.0f, 19.0f), &m_NumberTex.front());
		DrawFunc2D::DrawGraph(Vec2(1003.0f, 19.0f), m_NumberTex[10]);
		DrawFunc2D::DrawNumber2D(3, Vec2(1022.0f, 19.0f), &m_NumberTex.front());
	}

	// �E�F�[�u���`��
	DrawFunc2D::DrawNumber2D(m_NowWave, Vec2(1201.0f, 13.0f), &m_NumberTex_Battle.front());
	DrawFunc2D::DrawGraph(Vec2(1221.0f, 13.0f), m_NumberTex_Battle[10]);
	DrawFunc2D::DrawNumber2D(m_NowStage.m_Stage_Wave_Count, Vec2(1246.0f, 13.0f), &m_NumberTex_Battle.front());

	// �v���C���[�̓o�g�����ȊO�ł��`�悷��ׂ�����
	Pl->OnDraw();

	stage->Draw();
	block->Draw();

	Mgr.OnDraw();

}

void BattleScene::OnImguiDebug()
{
	Mgr.OnImguiDebug();

	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();
	using namespace KuroEngine;

	Vec2<float> mousePos = { 0.0f,0.0f };
	mousePos = input->GetMousePos();

	ImGui::Begin("attribute");
	if (attribute == BlockAttribute::attack1) {
		ImGui::Text("attack1");
	}else if (attribute == BlockAttribute::attack2) {
		ImGui::Text("attack2");
	}if (attribute == BlockAttribute::recovery) {
		ImGui::Text("recovery");
	}
	ImGui::End();
}

void BattleScene::OnFinalize()
{
	//m_Stage_End = false;
	En.clear();
	PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
	EnemyActions::EnemyActionMgr::Instance()->AllClear();
}

void BattleScene::SetStage(std::string StageName)
{
	m_NowStage = Stages::StageList::Instance()->GetStage(StageName);
}

void BattleScene::NextWave()
{
	// ���E�F�[�u�ɕύX
	m_NowWave++;
	m_Impossible_Put_Block_Timer = m_Impossible_Put_Block_Effect_Time;

	// ���݂��Ō�̃E�F�[�u�������ꍇ
	if (m_NowWave > m_NowStage.m_Stage_Wave_Count) {
		m_Stage_End = true;
		// �U�������N���A
		PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
		EnemyActions::EnemyActionMgr::Instance()->AllClear();
		return;
	}

	// ���E�F�[�u�̓G���擾
	En.clear();
	std::vector<EnemysData::EnemyData> EnemyData = m_NowStage.GetWaveEnemyIndex(m_NowWave);
	for (auto& data : EnemyData) {
		En.emplace_back(std::make_shared<Enemy>());
		GetUnitPtr<Enemy>(En.back())->SetEnemyData(data);
	}
	Mgr.SetUnits(Pl, En);
	if (En.size() == 1) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get());
	}
	if (En.size() == 2) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get());
	}
	if (En.size() == 3) {
		ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());
	}

	// �U�������N���A
	PlayerSkills::PlayerSkillMgr::Instance()->AllClear();
	EnemyActions::EnemyActionMgr::Instance()->AllClear();
}

BattleScene::BattleScene()
{
}

void BattleScene::PlayerTurn()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::SET_PRISM, OperationConfig::ON_TRIGGER)) {
		KuroEngine::Vec2<int> nowMapchip;
		std::vector<KuroEngine::Vec2<int>> shape;
		BlockColor color;

		//�u���b�N���擾
		block->GetBlock(&nowMapchip, &shape, &attribute, &color);
		//�z�u�\�Ȃ�z�u����
		if (!stage->JudgeSet(nowMapchip, shape, attribute, color)) { return; }
		//���̎g�p�u���b�N���Z�b�g����
		block->ChangeBlock();
	}
}
