#include "BattleScene.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

#include "ExistUnits.h"

void BattleScene::OnInitialize()
{
	// ---- �w�i
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_SukasiTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "Sukasi.png");
	m_BackTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "battle_scene_frame.png");



	Pl = std::make_shared<Player>();
	Pl->OnInitialize();
	Pl->StartTurn();


	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_1);
	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_2);
	En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_3);
	Mgr.OnInitialize(Pl, En);
	ExistUnits::Instance()->Set(Pl.get(), En[0].get(), En[1].get(), En[2].get());
	/*En.emplace_back(std::make_shared<Enemy>());
	GetUnitPtr<Enemy>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_Boss_1);
	Mgr.OnInitialize(Pl, En);
	ExistUnits::Instance()->Set(Pl.get(), En[0].get());*/

	stage.reset(new StageManager());
	stage->Initialize();
	block.reset(new BlockManager());
	//�����w��
	block->SetCenter(stage->GetCenter());
	block->Initialize();


	ExistUnits::Instance()->SetMapChipPtr(stage->GetMapChipPtr());
}

void BattleScene::OnUpdate()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	//�Z�b�g�\�Ȃ�Z�b�g����
	if (ExistUnits::Instance()->m_NowTurn == 0) {
		PlayerTurn();
	}

	//���Ɖ�̔���
	if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::X)) {
		//��m�F
		//std::vector<int> massNum;
		//std::vector<BlockColor> massColor;
		massColor.clear();
		massNum.clear();
		stage->MassProcess(&massNum, &massColor);
		//��m�F
		//int lineNum = 0;
		//std::vector<BlockColor> llineColor;
		lineColor.clear();

		stage->LineProcess(&lineNum, &lineColor);
	}

	if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::A)) {
		stage->Reset();
	}

	Mgr.OnUpdate();

	stage->Update();
	block->Update();
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

	ImGui::Begin("mass & line");
	ImGui::Text("mass");
	int mass = 0;
	for (int i = 0; i < massNum.size(); i++) {
		ImGui::Text(" %d : num %d |color %d", mass, massNum[i], massColor[i]);
		mass++;
	}
	ImGui::Text("line");
	int line = 0;
	for (int i = 0; i < lineColor.size(); i++) {
		ImGui::Text(" %d : color %d", line, lineColor[i]);
		line++;
	}
	ImGui::End();

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
}

BattleScene::BattleScene()
{
}

void BattleScene::PlayerTurn()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::B)) {
		KuroEngine::Vec2<int> nowMapchip;
		std::vector<KuroEngine::Vec2<int>> shape;
		BlockColor color;

		//�u���b�N���擾
		block->GetBlock(&nowMapchip, &shape, &attribute, &color);
		//�z�u�\�Ȃ�z�u����
		bool isSet = stage->JudgeSet(nowMapchip, shape, color);
		//���̎g�p�u���b�N���Z�b�g����
		block->SetIsChoice();
	}
}
