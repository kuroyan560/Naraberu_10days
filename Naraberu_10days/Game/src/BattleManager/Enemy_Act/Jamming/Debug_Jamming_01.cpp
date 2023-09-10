#include "Debug_Jamming_01.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include "../../ExistUnits.h"
#include "../../../Panel/PanelManager.h"

void Debug_Jamming_01::Action_Start()
{
	// 基底初期化
	E_Ac_Base::Action_Start();
}

void Debug_Jamming_01::Action_Update()
{
	// 終了処理
	if (GetEndTimer()) {
		Action_End();
		return;
	}

	// タイマー加算
	m_Timer++;

	using namespace KuroEngine;

	// お邪魔プリズムを設置
	// 行動は一回のみ
	if (m_Timer == 10) {
		for (int i = 0; i < 3; i++) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
		}
	}
}

void Debug_Jamming_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}

	// 仮で箱を描画
	//DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), Vec2(100.0f, 100.0f), Color(0, 255, 255, 255), true);
}

void Debug_Jamming_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
