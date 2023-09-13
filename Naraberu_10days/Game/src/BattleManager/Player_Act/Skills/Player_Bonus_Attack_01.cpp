#include "Player_Bonus_Attack_01.h"
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"
#include "../../ExistUnits.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include "../../../Effect/ScreenShakeManager.h"
#include "../../../RefreshRate.h"

#include"../../../SoundConfig.h"

void Player_Bonus_Attack_01::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Bonus_Attack_01::Action_Start()
{
	// 基底初期化
	P_Ac_Base::Action_Start();
}

void Player_Bonus_Attack_01::Action_Update()
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
			return;
		}

		m_BonusCount = ExistUnits::Instance()->m_NowBonusCount;

		// 全てのターゲットに攻撃する
		for (auto& tgt : m_Targets) {
			tgt->Damage(m_BonusCount);
			tgt->Damage(800);
		}
		if (m_BonusCount > 0) {
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}

		GetUnitPtr_nama<Player*>(m_Initiator)->EndBonusAttack();
	}
}

void Player_Bonus_Attack_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}
	if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
		return;
	}

	// 0ダメージの場合は描画しない
	if (m_BonusCount < 1) {
		return;
	}

	using namespace KuroEngine;
	for (auto& tgt : m_Targets) {
		if (GetUnitPtr_nama<Enemy*>(tgt)->m_Data.m_Name == "Boss_1") {
			GetUnitPtr_nama<Enemy*>(tgt)->Draw_Damage();
		}
		else if (int(float(m_Timer) / (20.0f * RefreshRate::RefreshRate_Mag)) % 2 == 0) {
			GetUnitPtr_nama<Enemy*>(tgt)->Draw_Damage();
		}
	}
}

void Player_Bonus_Attack_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
