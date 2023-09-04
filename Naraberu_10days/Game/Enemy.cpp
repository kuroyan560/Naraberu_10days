#include "Enemy.h"
#include "../../src/engine/FrameWork/UsersInput.h"


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
}
