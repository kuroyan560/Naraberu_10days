#include "BattleScene.h"

void BattleScene::OnInitialize()
{
	Pl.OnInitialize();
	Pl.StartTurn();


	En.emplace_back(new Enemy);
	GetUnitPtr<Enemy*>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_1);
	En.emplace_back(new Enemy);
	GetUnitPtr<Enemy*>(En[En.size() - 1])->SetEnemyData(EnemysData::DebugEnemy_2);

	Mgr.OnInitialize(&Pl, En);
}

void BattleScene::OnUpdate()
{
	Mgr.OnUpdate();
}

void BattleScene::OnDraw()
{
	using namespace KuroEngine;

	KuroEngineDevice::Instance()->Graphics().SetRenderTargets({
	D3D12App::Instance()->GetBackBuffRenderTarget()
		});
}

void BattleScene::OnImguiDebug()
{
	Mgr.OnImguiDebug();
}

void BattleScene::OnFinalize()
{
}

BattleScene::BattleScene()
{
}
