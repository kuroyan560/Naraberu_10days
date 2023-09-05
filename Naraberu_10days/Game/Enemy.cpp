#include "Enemy.h"
#include "../../src/engine/FrameWork/UsersInput.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "../../src/engine/FrameWork/WinApp.h"

#include "ExistUnits.h"

Enemy::Enemy()
{
	// ターン関連変数の初期化
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

	// タイマーの加算
	TimerUpdate();

	// 行動の決定
	if (!m_Now_Action && !m_Already_Act) {
		// とりあえず最初のアクションに決定しとく
		m_Action_Num = 0;
		// 必要な情報をセット(あとで分岐無しでいい設計にする)
		//m_Actions[m_Action_Num]->Action_Start();
		EnemyActions::EnemyActionMgr::Instance()->SetUnits(this, ExistUnits::Instance()->m_pPlayer);
		EnemyActions::EnemyActionMgr::Instance()->StartAction(m_Data.ActionList[1]);

		m_Now_Action = true;
	}
	// アクション中であれば
	if (m_Now_Action) {
		//m_Actions[m_Action_Num]->Action_Update();
		EnemyActions::EnemyActionMgr::Instance()->Update();

		// アクションが終わった
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
	// アクション中であれば
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
	// EnemyDataをコピーする
	m_Data = Data;
	m_Data.SetTexture();
	// HPをセットする
	m_HP = m_Data.m_HP;
	m_MaxHP = m_Data.m_MaxHP;
	// アクション名からアクションをセットする
	/*for (auto& data : m_Data.ActionList) {
		m_Actions.emplace_back(EnemyActions::SetActionOnName(data));
	}*/
}

void Enemy::Draw(int Index)
{
	using namespace KuroEngine;

	float IndexDiff = Index * 170.0f;

	// ユニットの描画
	DrawFunc2D::DrawExtendGraph2D(Vec2(977.0f, 115.0f + IndexDiff), Vec2(1222.0f, 215.0f + IndexDiff), m_Data.m_UnitTex);

	DrawFunc2D::DrawExtendGraph2D(Vec2(970.0f, 108.0f + IndexDiff), Vec2(1229.0f, 222.0f + IndexDiff), m_Data.m_FrameTex);
	DrawFunc2D::DrawExtendGraph2D(Vec2(1094.0f, 181.0f + IndexDiff), Vec2(1238.0f, 203.0f + IndexDiff), m_Data.m_HpFrameTex);

	// HPゲージが削れる演出用
	float HP_Gauge_Now_Value = float(m_HP);

	// タイマーが1以上　かつ　最大値以下なら減少演出
	if (m_HP_Break_Timer > 0 && m_HP_Break_Timer < m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer++;
		HP_Gauge_Now_Value = float(m_HP) + float(m_Before_HP - m_HP) * (1.0f - (float(m_HP_Break_Timer) / float(m_HP_GAUGE_BREAK_TIME)));
	}
	// 減少演出終わり
	if (m_HP_Break_Timer == m_HP_GAUGE_BREAK_TIME) {
		m_HP_Break_Timer = 0;
	}

	// 現在のHP割合
	float Now_HP_Rate = float(HP_Gauge_Now_Value) / float(m_MaxHP);
	// HPゲージの長さ
	float Gauge_Max_Width = 1247.0f - 1115.0f;
	// 現在のHPゲージの長さ
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
