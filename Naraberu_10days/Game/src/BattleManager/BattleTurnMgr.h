#pragma once
#include "UnitBase.h"
#include <vector>
#include <memory>

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

#include "CutIn/CutInMgr.h"
#include "../RefreshRate.h"

class BattleTurnMgr
{
	std::vector<std::shared_ptr<UnitBase>> UnitList;
	// 全体ターン数
	int m_Whole_Turn_Count;
	// 何番目のユニットのターンか
	int TurnNum;
	// ターンの経過フレーム
	int TurnFrameTime;
	// 次のバトルに移動する場合のタイマー
	int NextGameTimer;
	// 次のバトルに移動する場合のタイマーの終了値
	int NextGameTimeFinish;
	// 上のタイマーのデフォルト値
	const int NextGameTimeFinish_Default = 300;

	// 画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;

	// 敗北フラグ
	bool m_IsDefeat;

public:
	BattleTurnMgr() {
		m_Whole_Turn_Count = 0;
		TurnNum = 0;
		TurnFrameTime = 0;
		NextGameTimer = 0;
		NextGameTimeFinish = int(float(NextGameTimeFinish_Default) * RefreshRate::RefreshRate_Mag);
		m_IsDefeat = false;
	}

	void OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);
	// ウェーブに開始時にユニットをセット
	void SetUnits(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);

	void OnUpdate();
	void OnDraw();
	void OnImguiDebug();

	void NextTurnStart();

	// 次のバトルに行く場合の更新処理
	void Update_NextWave();
	// プレイヤーと敵が生きている場合の更新処理
	void Update_Battle();

	// 次のウェーブに移動するか
	bool ChangeNextWave() { return NextGameTimer >= NextGameTimeFinish; }

	// 生きている敵が居るか
	bool AliveEnemys();

	// プレイヤーが死んでおり、敗北が確定しているか
	bool GetDefeat() { return m_IsDefeat; }
};

