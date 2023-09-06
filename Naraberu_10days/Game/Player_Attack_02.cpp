#include "Player_Attack_02.h"
#include "Player.h"
#include "Enemy.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/FrameWork/WinApp.h"
#include "../../src/engine/FrameWork/UsersInput.h"

void Player_Attack_02::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Attack_02::Action_Start()
{
	// 基底初期化
	P_Ac_Base::Action_Start();
}

void Player_Attack_02::Action_Update()
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
		// 
		if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
			exit(1);
		}
		// 1番目のターゲットに攻撃する
		m_Targets[0]->Damage(m_Damage);

		// アルティメットポイントを加算
		GetUnitPtr_nama<Player*>(m_Initiator)->AddUltPoint(3);
	}
}

void Player_Attack_02::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// 仮で箱を描画
	KuroEngine::Vec2 LT = GetUnitPtr_nama<Enemy*>(m_Targets[0])->Get_Left_Top();
	KuroEngine::Vec2 RB = GetUnitPtr_nama<Enemy*>(m_Targets[0])->Get_Right_Bottom();
	DrawFunc2D::DrawBox2D(LT, RB, Color(255, 0, 0, 255), true);
}

void Player_Attack_02::Action_End()
{
	// 終了フラグを上げる
	End();
}
