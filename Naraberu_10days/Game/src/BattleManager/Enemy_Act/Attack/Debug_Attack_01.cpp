#include "Debug_Attack_01.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include"../../../SoundConfig.h"

void Debug_Attack_01::Action_Start()
{
	// 基底初期化
	E_Ac_Base::Action_Start();
}

void Debug_Attack_01::Action_Update()
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
		// 
		if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
			exit(1);
		}
		// 1番目のターゲットに攻撃する
		// 基礎攻撃力
		float Power = float(GetUnitPtr_nama<Enemy*>(m_Initiator)->GetEnemyData().m_ATK);
		int Damage_Result = int(Power * m_Data.m_AttackRate);
		m_Targets[0]->Damage(Damage_Result);
		SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
	}
}

void Debug_Attack_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}

	using namespace KuroEngine;
	// 仮で箱を描画
	//DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), Vec2(100.0f, 100.0f), Color(255, 0, 0, 255), true);
	
}

void Debug_Attack_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
