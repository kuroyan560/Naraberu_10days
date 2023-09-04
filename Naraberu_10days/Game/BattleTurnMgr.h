#pragma once
#include "UnitBase.h"
#include <vector>
#include <memory>

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

class BattleTurnMgr
{
	std::vector<std::shared_ptr<UnitBase>> UnitList;
	int TurnNum;

	int TurnFrameTime;

	// Player/Enemyターンの境目等に使用するフラグ
	bool NowEmptyTurn;
	// カットイン
	int CutInTimer;
	const int CutInEndTime = 200;
	float inQuart(float time, float begin, float change, float duration) {
		return change * powf((time / duration), 4) + begin;
	}
	float outQuart(float time, float begin, float change, float duration) {
		return -change * (powf((time / duration - 1), 4) - 1) + begin;
	}
	float OutInQuart(float time, float begin, float change, float duration) {
		if (time / duration < 0.5f) {
			return outQuart(time * 2, begin, change / 2, duration);
		}
		return inQuart((time * 2) - duration, begin + change / 2, change / 2, duration);
	}


	// 画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;

private:
	void StartCutIn(int CutInNum); // 0 : playerturn, 1 : enemytrun


public:
	void OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);

	void OnUpdate();
	void OnDraw();
	void OnImguiDebug();

	void NextTurnStart();

	bool NowCutIn() { return CutInTimer > 0 ? true : false; }
};

