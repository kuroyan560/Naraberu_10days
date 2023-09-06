#include "Player_Heal_01.h"
#include "Player.h"
#include "Enemy.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

void Player_Heal_01::Param_Set(int Amount, int Duration)
{
	m_Amount = Amount;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Heal_01::Action_Start()
{
	// 基底初期化
	P_Ac_Base::Action_Start();
}

void Player_Heal_01::Action_Update()
{
	// 終了処理
	if (GetEndTimer()) {
		Action_End();
		return;
	}

	// タイマー加算
	m_Timer++;

	// 攻撃は一回のみ
	if (m_Timer == 1) {
		// 自分を回復する
		m_Initiator->Heal(m_Amount);
	}
}

void Player_Heal_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// 仮で箱を描画
	KuroEngine::Vec2 LT = GetUnitPtr_nama<Player*>(m_Initiator)->Get_Left_Top();
	KuroEngine::Vec2 RB = GetUnitPtr_nama<Player*>(m_Initiator)->Get_Right_Bottom();
	DrawFunc2D::DrawBox2D(LT, RB, Color(0, 230, 30, 255), true);
}

void Player_Heal_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
