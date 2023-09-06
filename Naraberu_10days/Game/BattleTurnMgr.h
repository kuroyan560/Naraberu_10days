#pragma once
#include "UnitBase.h"
#include <vector>
#include <memory>

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

#include "CutInMgr.h"

class BattleTurnMgr
{
	std::vector<std::shared_ptr<UnitBase>> UnitList;
	// 全体ターン数
	int m_Whole_Turn_Count;
	// 何番目のユニットのターンか
	int TurnNum;
	// ターンの経過フレーム
	int TurnFrameTime;

	// 画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;

public:
	BattleTurnMgr() {
		m_Whole_Turn_Count = 0;
		TurnNum = 0;
		TurnFrameTime = 0;
	}

	void OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);

	void OnUpdate();
	void OnDraw();
	void OnImguiDebug();

	void NextTurnStart();

	// プレイヤーと敵が生きている場合の更新処理
	void Update_Battle();
};

