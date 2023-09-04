#include "Enemy.h"
#include "../../src/engine/FrameWork/UsersInput.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "../../src/engine/FrameWork/WinApp.h"

Enemy::Enemy()
{
	// ターン関連変数の初期化
	m_isMyTurn = false;
	m_NextTurn = false;
}

void Enemy::OnInitialize()
{
}

void Enemy::OnUpdate()
{
	using namespace KuroEngine;
	if (UsersInput::Instance()->KeyOnTrigger(DIK_SPACE)) {
		EndTurn();
	}
}

void Enemy::OnAlwaysUpdate()
{
	using namespace KuroEngine;
	if (UsersInput::Instance()->KeyInput(DIK_B)) {
		m_Data.m_HP > 0 ? m_Data.m_HP-- : 0;
	}
	if (UsersInput::Instance()->KeyInput(DIK_N)) {
		m_Data.m_HP < m_Data.m_MaxHP ? m_Data.m_HP++ : 0;
	}
}

void Enemy::OnDraw()
{
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
}

void Enemy::Draw(int Index)
{
	using namespace KuroEngine;

	float IndexDiff = Index * 170.0f;

	DrawFunc2D::DrawExtendGraph2D(Vec2(985.0f, 107.0f + IndexDiff), Vec2(1244.0f, 221.0f + IndexDiff), m_Data.m_FrameTex);
	DrawFunc2D::DrawExtendGraph2D(Vec2(1109.0f, 180.0f + IndexDiff), Vec2(1253.0f, 202.0f + IndexDiff), m_Data.m_HpFrameTex);

	// 現在のHP割合
	float Now_HP_Rate = float(m_Data.m_HP) / float(m_Data.m_MaxHP);
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
		Vec2(1115.0f, 186.0f + IndexDiff), Vec2(1247.0f, 196.0f + IndexDiff), HP_Gauge,
		Vec2(1115.0f, 186.0f + IndexDiff), Vec2(1115.0f + Gauge_Width, 196.0f + IndexDiff));


}
