#include "Player_Attack_02.h"
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include "../../../Effect/ScreenShakeManager.h"
#include "../../../RefreshRate.h"

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
		//m_Targets[0]->Damage(m_Damage);
		// 全てのターゲットに攻撃する
		for (auto& tgt : m_Targets) {
			//tgt->Damage(m_Damage * 2000);
			tgt->Damage(m_Damage);
		}
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
	if (int(float(m_Timer) / (20.0f * RefreshRate::RefreshRate_Mag)) % 2 == 0) {
		for (auto& tgt : m_Targets) {
			GetUnitPtr_nama<Enemy*>(tgt)->Draw_Damage();
			/*KuroEngine::Vec2 LT = GetUnitPtr_nama<Enemy*>(tgt)->Get_Left_Top();
			KuroEngine::Vec2 RB = GetUnitPtr_nama<Enemy*>(tgt)->Get_Right_Bottom();
			DrawFunc2D::DrawBox2D(LT + ScreenShakeManager::Instance()->GetOffset()
				, RB + ScreenShakeManager::Instance()->GetOffset(), Color(255, 0, 0, 255), true);*/
		}
	}
}

void Player_Attack_02::Action_End()
{
	// 終了フラグを上げる
	End();
}
