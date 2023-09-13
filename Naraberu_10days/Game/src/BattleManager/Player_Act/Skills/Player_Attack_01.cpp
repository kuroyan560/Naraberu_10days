#include "Player_Attack_01.h"
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"
#include "../../../Effect/ScreenShakeManager.h"
#include "../../../RefreshRate.h"
#include"../../../SoundConfig.h"

void Player_Attack_01::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Attack_01::Action_Start()
{
	// 基底初期化
	P_Ac_Base::Action_Start();
}

void Player_Attack_01::Action_Update()
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
			//exit(1);
			return;
		}
		// 1番目のターゲットに攻撃する
		m_Targets[0]->Damage(m_Damage * 2);
		SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
	}
}

void Player_Attack_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}
	if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
		return;
	}

	using namespace KuroEngine;
	
	if (GetUnitPtr_nama<Enemy*>(m_Targets[0])->m_Data.m_Name == "Boss_1") {
		GetUnitPtr_nama<Enemy*>(m_Targets[0])->Draw_Damage();
	}
	else if (int(float(m_Timer) / (20.0f * RefreshRate::RefreshRate_Mag)) % 2 == 0) {
		GetUnitPtr_nama<Enemy*>(m_Targets[0])->Draw_Damage();
	}
}

void Player_Attack_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
