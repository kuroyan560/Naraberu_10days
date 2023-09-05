#include "Enemy.h"
#include "../../src/engine/FrameWork/UsersInput.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "../../src/engine/FrameWork/WinApp.h"

#include "ExistUnits.h"

Enemy::Enemy()
{
	// �^�[���֘A�ϐ��̏�����
	m_isMyTurn = false;
	m_NextTurn = false;
	m_Action_Num = 0;
	m_Now_Action = false;
	m_Already_Act = false;
	m_Timer = 0;
}

void Enemy::OnInitialize()
{
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
		EnemyActions::EnemyActionMgr::Instance()->SetUnits(this, ExistUnits::Instance()->m_pPlayer);
		EnemyActions::EnemyActionMgr::Instance()->StartAction(m_Data.ActionList[1]);

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
	if (UsersInput::Instance()->KeyInput(DIK_B)) {
		m_HP > 0 ? m_HP-- : 0;
	}
	if (UsersInput::Instance()->KeyInput(DIK_N)) {
		m_HP < m_MaxHP ? m_HP++ : 0;
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

void Enemy::Draw(int Index)
{
	using namespace KuroEngine;

	float IndexDiff = Index * 170.0f;

	// ���j�b�g�̕`��
	DrawFunc2D::DrawExtendGraph2D(Vec2(977.0f, 115.0f + IndexDiff), Vec2(1222.0f, 215.0f + IndexDiff), m_Data.m_UnitTex);

	DrawFunc2D::DrawExtendGraph2D(Vec2(970.0f, 108.0f + IndexDiff), Vec2(1229.0f, 222.0f + IndexDiff), m_Data.m_FrameTex);
	DrawFunc2D::DrawExtendGraph2D(Vec2(1094.0f, 181.0f + IndexDiff), Vec2(1238.0f, 203.0f + IndexDiff), m_Data.m_HpFrameTex);

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
	// HP�Q�[�W�̒���
	float Gauge_Max_Width = 1247.0f - 1115.0f;
	// ���݂�HP�Q�[�W�̒���
	float Gauge_Width = Gauge_Max_Width * Now_HP_Rate;


	std::shared_ptr<KuroEngine::TextureBuffer> HP_Gauge = m_Data.m_HpTex_green;
	if (Now_HP_Rate <= 0.2f) {
		HP_Gauge = m_Data.m_HpTex_red;
	}
	else if (Now_HP_Rate <= 0.5f) {
		HP_Gauge = m_Data.m_HpTex_yellow;
	}

	DrawFunc2D_Mask::DrawExtendGraph2D(
		Vec2(1100.0f, 187.0f + IndexDiff), Vec2(1232.0f, 197.0f + IndexDiff), HP_Gauge,
		Vec2(1100.0f, 187.0f + IndexDiff), Vec2(1100.0f + Gauge_Width, 197.0f + IndexDiff));


}
