#pragma once
#include "UnitBase.h"
#include <vector>
#include <memory>
#include <Windows.h>

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

#include "CutIn/CutInMgr.h"
#include "../RefreshRate.h"

class BattleTurnMgr
{
	// 全体ターン数 (個別合算)
	int m_Whole_Turn_Count;
	// 全体ターン数 (個別合算)
	int m_Total_Turn_Count;
	// 何番目のユニットのターンか
	int TurnNum;
	// ターンの経過フレーム
	int TurnFrameTime;
	// 次のバトルに移動する場合のタイマー
	int NextGameTimer;
	// 次のバトルに移動する場合のタイマーの終了値
	int NextGameTimeFinish;
	// 上のタイマーのデフォルト値
	const int NextGameTimeFinish_Default = 150;

	// 画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;
	// ターンエンド
	bool m_Selected_TurnEnd;
	bool m_Checked_TurnEnd;
	int m_Selected_TurnEnd_Timer;
	float m_Scaling_Timer;
	float m_Scaling_Timer_Max;
	bool m_Moving_Flag;
	float m_Moving_Timer;
	float m_Moving_Timer_Max;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEndTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEnd_EnterTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEnd_Crtl_EnterTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEnd_SelectedTex;
	void TurnEndButtonUpdate();
	void TurnEndButtonDraw();
	float ResultEasing(float time);

	// 敗北フラグ
	bool m_IsDefeat;

	// ターンエンド時間
	SYSTEMTIME StartTime;
	SYSTEMTIME NowTime;
	__int64 m_ProgressTime;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Timer_Frame_Tex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Timer_Gauge_Tex;
public:
	SYSTEMTIME PauseStartTime;
	SYSTEMTIME PauseEndTime;
	__int64 m_PauseTime;
	std::vector<__int64> m_PauseTimeContainer;

public:
	std::vector<std::shared_ptr<UnitBase>> UnitList;

	BattleTurnMgr();

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

	// 合計ターン取得
	int GetTotalTurn() { return m_Total_Turn_Count; }

	// ターンエンドボタン選択中か
	bool GetSelectedTurnEnd() { return m_Selected_TurnEnd; }

	// 時間ゲージ描画
	void AutoTurnEndTimerDraw();

	// 現在ターン取得
	int GetNowTurn() { return TurnNum; }
};

