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
	int TurnNum;

	int TurnFrameTime;

	// ‰æ‘œ
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;

public:
	void OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);

	void OnUpdate();
	void OnDraw();
	void OnImguiDebug();

	void NextTurnStart();

};

