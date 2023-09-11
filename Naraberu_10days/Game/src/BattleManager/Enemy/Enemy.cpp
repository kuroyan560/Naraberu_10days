#include "Enemy.h"
#include "FrameWork/UsersInput.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "FrameWork/WinApp.h"

#include "../ExistUnits.h"
#include "../Reticle/Reticle.h"
#include "../../RefreshRate.h"

#include"../../OperationConfig.h"

#include "../../Effect/ScreenShakeManager.h"

Enemy::Enemy()
{
	// �^�[���֘A�ϐ��̏�����
	m_isMyTurn = false;
	m_NextTurn = false;
	m_Action_Num = 0;
	m_Now_Action = false;
	m_Already_Act = false;
	m_Timer = 0;
	m_Tutorial_Act_Count = 0;
}

void Enemy::OnInitialize()
{
	m_Tutorial_Act_Count = 0;
}

void Enemy::OnUpdate()
{
	using namespace KuroEngine;
	/*if (UsersInput::Instance()->KeyOnTrigger(DIK_SPACE)) {
		EndTurn();
	}*/

	// �^�C�}�[�̉��Z
	TimerUpdate();

	// �s���̌���
	if (!m_Now_Action && !m_Already_Act) {
		// �Ƃ肠�����ŏ��̃A�N�V�����Ɍ��肵�Ƃ�
		m_Action_Num = 0;
		// �K�v�ȏ����Z�b�g(���Ƃŕ��򖳂��ł����݌v�ɂ���)
		//m_Actions[m_Action_Num]->Action_Start();
		if (m_Data.m_AI_Type == EnemysData::AI_TYPE::DEFAULT) {
			EnemyActions::EnemyActionMgr::Instance()->SetUnits(this, ExistUnits::Instance()->m_pPlayer);
			EnemyActions::EnemyActionMgr::Instance()->StartAction(m_Data.ActionList[GetRand(0, int(m_Data.ActionList.size()))]);
		}
		if (m_Data.m_AI_Type == EnemysData::AI_TYPE::TUTORIAL) {
			EnemyActions::EnemyActionMgr::Instance()->SetUnits(this, ExistUnits::Instance()->m_pPlayer);
			EnemyActions::EnemyActionMgr::Instance()->StartAction(m_Data.ActionList[0]);
		}

		m_Now_Action = true;
	}
	// �A�N�V�������ł����
	if (m_Now_Action) {
		//m_Actions[m_Action_Num]->Action_Update();
		EnemyActions::EnemyActionMgr::Instance()->Update();

		// �A�N�V�������I�����
		if (EnemyActions::EnemyActionMgr::Instance()->GetEnd()) {
			m_Now_Action = false;
			m_Already_Act = true;
			m_Timer = 0;
		}
	}
	if (m_Already_Act == true) {
		m_Timer++;
		if (m_Timer >= 100) {
			EndTurn();
			m_Already_Act = false;
		}
	}
}

void Enemy::OnAlwaysUpdate()
{
	using namespace KuroEngine;
	if (OperationConfig::Instance()->DebugKeyInput(DIK_G)) {
		m_HP > 0 ? m_HP-- : 0;
	}
	if (OperationConfig::Instance()->DebugKeyInput(DIK_H)) {
		m_HP < m_MaxHP ? m_HP++ : 0;
	}
	if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_M)) {
		Damage(m_HP / 2);
	}
}

void Enemy::OnDraw()
{
	// �A�N�V�������ł����
	if (m_Now_Action) {
		//m_Actions[m_Action_Num]->Action_Draw();
		EnemyActions::EnemyActionMgr::Instance()->Draw();
	}
}

void Enemy::OnImguiDebug()
{
}

void Enemy::OnFinalize()
{
}

void Enemy::SetEnemyData(EnemysData::EnemyData Data)
{
	// EnemyData���R�s�[����
	m_Data = Data;
	m_Data.SetTexture();
	// HP���Z�b�g����
	m_HP = m_Data.m_HP;
	m_MaxHP = m_Data.m_MaxHP;
	// �A�N�V����������A�N�V�������Z�b�g����
	/*for (auto& data : m_Data.ActionList) {
		m_Actions.emplace_back(EnemyActions::SetActionOnName(data));
	}*/
}

void Enemy::Draw(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn, int NowTarget)
{

	if (m_Data.m_Tag == EnemysData::ENEMY_TAG::BOSS) {
		Draw_Boss(Index, NowTurn_Index, Index_Max, Dark, FrameTime, FirstTurn);
		return;
	}

	using namespace KuroEngine;

	float IndexDiff = Index * 170.0f;

	// �G�̑��� (���j�b�g���̓v���C���[�G���Z���� 1 �X�^�[�g�Ȃ̂� -2 ���Ă���)
	int EnemyCount = Index_Max - 2;

	// �^�[�����ɍ��ɓ�����
	static const int POS_FINISH_FRAME_TIME = int(10.0f * float(RefreshRate::RefreshRate_Mag)); // �������̂ɂ����鎞��
	static const float MOVE_WIDTH_MAX = 20.0f; // �������ő�l
	float Move_Width = 0.0f; // ���ݓ������Ă���l
	// �o�߃t���[���{���v�Z
	float Progress_Frame_Pos = float(FrameTime > POS_FINISH_FRAME_TIME ? POS_FINISH_FRAME_TIME : FrameTime) / float(POS_FINISH_FRAME_TIME);
	// ���^�[���� (Index - 1�œn����Ă�̂ŉ��Z�����Ƃ�)
	if (Index + 1 == NowTurn_Index) {
		Move_Width = MOVE_WIDTH_MAX * Progress_Frame_Pos;
	}
	// �O�������̃^�[�����������ɉE�Ɉړ������� (���̈ʒu�ɖ߂����o) Index��0���ƑO�^�[���̓v���C���[�Ȃ̂ŃX�L�b�v
	if (NowTurn_Index != 0 && Index + 1 == NowTurn_Index - 1) {
		Move_Width = MOVE_WIDTH_MAX - MOVE_WIDTH_MAX * Progress_Frame_Pos;
	}
	// �Ō�̓G�^�[������v���C���[�^�[���Ɉڍs���鎞 ����̓s����ŏ��̃^�[���ɂ��ʂ��Ă��܂��׍ŏ��̃^�[�������O���锻������Ă���
	if (NowTurn_Index == 0 && Index == EnemyCount && FirstTurn != 0) {
		Move_Width = MOVE_WIDTH_MAX - MOVE_WIDTH_MAX * Progress_Frame_Pos;
	}

	// ��h��
	int Mask_Black = 105; // �ŏI�I��RGB�{���Ƃ̍�
	Color Mask = Color(255 - Mask_Black, 255 - Mask_Black, 255 - Mask_Black, 255);
	// ���X�ɈÂ�����
	static const int COL_FINISH_FRAME_TIME = int(25.0f * float(RefreshRate::RefreshRate_Mag)); // �Â�����̂ɂ����鎞��
	float Progress_Frame_Color = float(FrameTime > COL_FINISH_FRAME_TIME ? COL_FINISH_FRAME_TIME : FrameTime) / float(COL_FINISH_FRAME_TIME);
	// �Â�����ꍇ
	if (Dark) {
		int Col = int(255.0f - (float(Mask_Black) * float(Progress_Frame_Color)));

		// �G���j�b�g�̍ŏ��̃^�[���̏ꍇ
		if (NowTurn_Index == 1) {
			if (Index != 0) {
				int Col = int(255.0f - (float(Mask_Black) * float(Progress_Frame_Color)));
				Mask = Color(Col, Col, Col, 255);
			}
		}

		// ���^�[���� (Index - 1�œn����Ă�̂ŉ��Z�����Ƃ�) ���� �O�^�[�����v���C���[�Ȃ疾�邢�܂܂Ȃ̂ŃX�L�b�v
		if (Index + 1 == NowTurn_Index && NowTurn_Index != 0) {
			int BlackCol = 255 - Mask_Black;
			BlackCol = int(float(BlackCol) + float(Mask_Black) * float(Progress_Frame_Color));
			Mask = Color(BlackCol, BlackCol, BlackCol, 255);
		}
		// �O�������̃^�[�����������ɉE�Ɉړ������� (���̈ʒu�ɖ߂����o) NowTurn_Index��0���ƑO�^�[���̓v���C���[�Ȃ̂ŃX�L�b�v
		if (NowTurn_Index != 0 && Index + 1 == NowTurn_Index - 1) {
			Mask = Color(Col, Col, Col, 255);
		}
		// �Ō�̓G�^�[������v���C���[�^�[���Ɉڍs���鎞 ����̓s����ŏ��̃^�[���ɂ��ʂ��Ă��܂��׍ŏ��̃^�[�������O���锻������Ă���
		if (NowTurn_Index == 0 && Index == EnemyCount && FirstTurn != 0) {
			Mask = Color(Col, Col, Col, 255);
		}
	}
	else {
		Mask = Color(255, 255, 255, 255);
		// �v���C���[�^�[���̏ꍇ
		if (NowTurn_Index == 0) {
			//if (Index == EnemyCount) {
			//	Mask = Color(255, 255, 255, 255);
			//}
			//else {
				int BlackCol = 255 - Mask_Black;
				BlackCol = int(float(BlackCol) + float(Mask_Black) * float(Progress_Frame_Color));
				Mask = Color(BlackCol, BlackCol, BlackCol, 255);
			//}
		}
	}

	// ����ł��狭���I�ɈÂ�����
	// ����ł��狭���I��Move_Width�������Ȃ��悤��
	if (m_HP <= 0) {
		Mask = Color(255 - Mask_Black, 255 - Mask_Black, 255 - Mask_Black, 255);
		Move_Width = 0.0f;
	}
	
	std::shared_ptr<KuroEngine::TextureBuffer> FrameTex = m_Data.m_FrameTex;
	std::shared_ptr<KuroEngine::TextureBuffer> HPFrameTex = m_Data.m_HpFrameTex;
	if (NowTarget == Index) {
		FrameTex = m_Data.m_FrameTex_Target;
		HPFrameTex = m_Data.m_HpFrameTex_Target;
	}

	D_Move_Width = Move_Width;
	D_IndexDiff = IndexDiff;
	// ���j�b�g�̕`��
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(1009.0f - Move_Width, 115.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1254.0f - Move_Width, 215.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), m_Data.m_UnitTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(1002.0f - Move_Width, 108.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1261.0f - Move_Width, 222.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), FrameTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(1126.0f - Move_Width, 181.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1270.0f - Move_Width, 203.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), HPFrameTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);

	std::shared_ptr<KuroEngine::TextureBuffer> IndexTex = m_Data.m_IndexTex_1;
	if (NowTarget == Index) {
		IndexTex = m_Data.m_IndexTex_1_Target;
		if (Index == 1) IndexTex = m_Data.m_IndexTex_2_Target;
		else if (Index == 2) IndexTex = m_Data.m_IndexTex_3_Target;
	}
	else {
		if (Index == 1) IndexTex = m_Data.m_IndexTex_2;
		else if (Index == 2) IndexTex = m_Data.m_IndexTex_3;
	}

	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(1185.0f - Move_Width, 117.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1252.0f - Move_Width, 177.0f + IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), IndexTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);

	//DrawFunc2D::DrawExtendGraph2D(Vec2(977.0f, 115.0f + IndexDiff), Vec2(1222.0f, 215.0f + IndexDiff), m_Data.m_UnitTex);
	//DrawFunc2D::DrawExtendGraph2D(Vec2(970.0f, 108.0f + IndexDiff), Vec2(1229.0f, 222.0f + IndexDiff), m_Data.m_FrameTex);
	//DrawFunc2D::DrawExtendGraph2D(Vec2(1094.0f, 181.0f + IndexDiff), Vec2(1238.0f, 203.0f + IndexDiff), m_Data.m_HpFrameTex);

	// �`��ʒu�̕ۑ�
	m_Left_Top = Vec2(1002.0f - Move_Width, 108.0f + IndexDiff);
	m_Right_Bottom = Vec2(1261.0f - Move_Width, 222.0f + IndexDiff);
	m_Center = m_Left_Top + ((m_Right_Bottom - m_Left_Top) / 2.0f);

	// ���e�B�N���̕`��
	if (NowTarget == Index) {
		/*DrawFunc2D_Color::DrawExtendGraph2D(Vec2(m_Left_Top.x - 48 + 10, m_Right_Bottom.y - 48 - 10), Vec2(m_Left_Top.x + 48 + 10, m_Right_Bottom.y + 48 - 10), m_Data.m_ReticleTex, Mask,
			{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);*/
		Reticle::Instance()->Draw(Index, m_Left_Top, m_Right_Bottom, Mask);
	}



}

void Enemy::Draw_Boss(int Index, int NowTurn_Index, int Index_Max, bool Dark, int FrameTime, bool FirstTurn)
{
	using namespace KuroEngine;

	Color Mask = Color(255, 255, 255, 255);
	// ���j�b�g�̕`��
	/*DrawFunc2D_Color::DrawExtendGraph2D(Vec2(977.0f, 115.0f), Vec2(1222.0f, 215.0f), m_Data.m_UnitTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(970.0f - Move_Width, 108.0f + IndexDiff), Vec2(1229.0f - Move_Width, 222.0f + IndexDiff), m_Data.m_FrameTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(1094.0f - Move_Width, 181.0f + IndexDiff), Vec2(1238.0f - Move_Width, 203.0f + IndexDiff), m_Data.m_HpFrameTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);*/

	int Mask_Black = 105; // �ŏI�I��RGB�{���Ƃ̍�
	// ����ł��狭���I�ɈÂ�����
	if (m_HP <= 0) {
		Mask = Color(255 - Mask_Black, 255 - Mask_Black, 255 - Mask_Black, 255);
	}

	Vec2 Window_Size = WinApp::Instance()->GetExpandWinSize();
	//DrawFunc2D::DrawExtendGraph2D(Vec2(0.0f, 0.0f), Vec2(Window_Size.x, Window_Size.y), m_Data.m_FrameTex);
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(0.0f, 0.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(Window_Size.x, Window_Size.y) + ScreenShakeManager::Instance()->GetOffset(), m_Data.m_FrameTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	DrawFunc2D_Color::DrawExtendGraph2D(Vec2(912.0f, 410.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1269.0f, 595.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Data.m_HpFrameTex, Mask,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);

	// �`��ʒu�̕ۑ�
	m_Left_Top = Vec2(926.0f, 60.0f);
	m_Right_Bottom = Vec2(1247.0f, 610.0f);
	m_Center = m_Left_Top + ((m_Right_Bottom - m_Left_Top) / 2.0f);	

	// HP�Q�[�W�����鉉�o�p
	float HP_Gauge_Now_Value = float(m_HP);

	// �^�C�}�[��1�ȏ�@���@�ő�l�ȉ��Ȃ猸�����o
	if (m_HP_Break_Timer > 0 && m_HP_Break_Timer < m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer++;
		HP_Gauge_Now_Value = float(m_HP) + float(m_Before_HP - m_HP) * (1.0f - (float(m_HP_Break_Timer) / float(m_HP_GAUGE_BREAK_TIME)));
	}
	// �������o�I���
	if (m_HP_Break_Timer == m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer = 0;
	}

	// ���݂�HP����
	float Now_HP_Rate = float(HP_Gauge_Now_Value) / float(m_MaxHP);
	float Now_HP_Rate2 = float(m_HP) / float(m_MaxHP);
	// HP�Q�[�W�̒���
	float Gauge_Max_Width = 1258.0f - 923.0f;
	// ���݂�HP�Q�[�W�̒���
	float Gauge_Width = Gauge_Max_Width * Now_HP_Rate;
	float Gauge_Width2 = Gauge_Max_Width * Now_HP_Rate2;

	std::shared_ptr<KuroEngine::TextureBuffer> HP_Gauge = m_Data.m_HpTex_green;
	if (Now_HP_Rate <= 0.2f) {
		HP_Gauge = m_Data.m_HpTex_red;
	}
	else if (Now_HP_Rate <= 0.5f) {
		HP_Gauge = m_Data.m_HpTex_yellow;
	}

	using namespace KuroEngine;

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(923.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1258.0f, 580.0f) + ScreenShakeManager::Instance()->GetOffset(), m_Data.m_HpTex_break,
		Vec2(923.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(923.0f + Gauge_Width, 580.0f) + ScreenShakeManager::Instance()->GetOffset());

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(923.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1258.0f, 580.0f) + ScreenShakeManager::Instance()->GetOffset(), HP_Gauge,
		Vec2(923.0f, 427.0f) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(923.0f + Gauge_Width2, 580.0f) + ScreenShakeManager::Instance()->GetOffset());
}

void Enemy::Draw_Damage()
{
	using namespace KuroEngine;
	DrawFunc2D::DrawExtendGraph2D(Vec2(1009.0f - D_Move_Width, 115.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1254.0f - D_Move_Width, 215.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), m_Data.m_UnitTex_Damage, 0.6f);
}

void Enemy::DrawHpGauge()
{
	using namespace KuroEngine;

	// HP�Q�[�W�����鉉�o�p
	float HP_Gauge_Now_Value = float(m_HP);

	// �^�C�}�[��1�ȏ�@���@�ő�l�ȉ��Ȃ猸�����o
	if (m_HP_Break_Timer > 0 && m_HP_Break_Timer < m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer++;
		HP_Gauge_Now_Value = float(m_HP) + float(m_Before_HP - m_HP) * (1.0f - (float(m_HP_Break_Timer) / float(m_HP_GAUGE_BREAK_TIME)));
	}
	// �������o�I���
	if (m_HP_Break_Timer == m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer = 0;
	}

	// ���݂�HP����
	float Now_HP_Rate = float(HP_Gauge_Now_Value) / float(m_MaxHP);
	float Now_HP_Rate2 = float(m_HP) / float(m_MaxHP);
	// HP�Q�[�W�̒���
	float Gauge_Max_Width = 1247.0f - 1115.0f;
	// ���݂�HP�Q�[�W�̒���
	float Gauge_Width = Gauge_Max_Width * Now_HP_Rate;
	float Gauge_Width2 = Gauge_Max_Width * Now_HP_Rate2;

	std::shared_ptr<KuroEngine::TextureBuffer> HP_Gauge = m_Data.m_HpTex_green;
	if (Now_HP_Rate <= 0.2f) {
		HP_Gauge = m_Data.m_HpTex_red;
	}
	else if (Now_HP_Rate <= 0.5f) {
		HP_Gauge = m_Data.m_HpTex_yellow;
	}

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(1132.0f - D_Move_Width, 187.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1264.0f - D_Move_Width, 197.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), m_Data.m_HpTex_break,
		Vec2(1132.0f - D_Move_Width, 187.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1132.0f - D_Move_Width + Gauge_Width, 197.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset());

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(1132.0f - D_Move_Width, 187.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1264.0f - D_Move_Width, 197.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset(), HP_Gauge,
		Vec2(1132.0f - D_Move_Width, 187.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(1132.0f - D_Move_Width + Gauge_Width2, 197.0f + D_IndexDiff) + ScreenShakeManager::Instance()->GetOffset());
}
