#include "Debug_Heal_01.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"
#include "../../ExistUnits.h"
#include"../../../SoundConfig.h"

void Debug_Heal_01::Action_Start()
{
	// 基底初期化
	E_Ac_Base::Action_Start();
}

void Debug_Heal_01::Action_Update()
{
	// 終了処理
	if (GetEndTimer()) {
		Action_End();
		return;
	}

	// タイマー加算
	m_Timer++;

	// 攻撃は一回のみ
	if (m_Timer == 20) {
		// 対象が見つからない
		if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
			exit(1);
		}
		// 全体を回復する
		m_Initiator->Heal_Par(int(m_Data.m_HealRate));
		
		for (auto& tgt : ExistUnits::Instance()->m_Enemys) {
			tgt->Heal_Par(int(m_Data.m_HealRate));
			//tgt->Damage(800);
		}
		
		//m_Targets[0]->Damage(int(m_Data.m_HealRate));
		SoundConfig::Instance()->Play(SoundConfig::SE_HEAL);
	}
}

void Debug_Heal_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// 仮で箱を描画
	//DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), Vec2(100.0f, 100.0f), Color(0, 255, 0, 255), true);
}

void Debug_Heal_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
