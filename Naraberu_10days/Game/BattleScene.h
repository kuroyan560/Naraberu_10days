#pragma once
#include "KuroEngineDevice.h"

#include "Player.h" 
#include "Enemy.h"
#include "BattleTurnMgr.h"
#include "BlockManager.h"
#include "StageManager.h"

class BattleScene :
    public KuroEngine::BaseScene
{
	// プレイヤー
	std::shared_ptr<Player> Pl;
	// 敵リスト
	std::vector<std::shared_ptr<UnitBase>> En;
	// バトル
	BattleTurnMgr Mgr;
	// 背景
	std::shared_ptr<KuroEngine::TextureBuffer> m_SukasiTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_BackTex;
	//ブロック
	std::shared_ptr<BlockManager> block;
	//ステージ
	std::shared_ptr<StageManager> stage;

	BlockAttribute attribute;


	void PlayerTurn();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	BattleScene();

};

