#include "Player_Ultimate_01.h"
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"

#include "../../../Effect/ScreenShakeManager.h"
#include "../../../RefreshRate.h"

#include"../../../SoundConfig.h"

void Player_Ultimate_01::Param_Set(int Damage, int Duration)
{
	m_Damage = Damage;
	m_Timer = 0;
	m_Finish = Duration;
}


void Player_Ultimate_01::Action_Start()
{
	// 基底初期化
	P_Ac_Base::Action_Start();
}

void Player_Ultimate_01::Action_Update()
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
		// 全てのターゲットに攻撃する
		for (auto& tgt : m_Targets) {
			tgt->Damage(m_Damage);
		}

		SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		// アルティメットポイントを加算
		//GetUnitPtr_nama<Player*>(m_Initiator)->AddUltPoint(1);
	}
}

void Player_Ultimate_01::Action_Draw()
{
	// 終了済みの場合は描画しない
	if (GetEnd()) {
		return;
	}
	if (m_Targets.size() < 1 || m_Targets[0] == nullptr) {
		return;
	}

	using namespace KuroEngine;
	// 仮で箱を描画
	if (int(float(m_Timer) / (20.0f * RefreshRate::RefreshRate_Mag)) % 2 == 0) {
		for (auto& tgt : m_Targets) {
			GetUnitPtr_nama<Enemy*>(tgt)->Draw_Damage();
		}
	}
	/*for (auto& tgt : m_Targets) {
		KuroEngine::Vec2 LT = GetUnitPtr_nama<Enemy*>(tgt)->Get_Left_Top();
		KuroEngine::Vec2 RB = GetUnitPtr_nama<Enemy*>(tgt)->Get_Right_Bottom();
		DrawFunc2D::DrawBox2D(LT + ScreenShakeManager::Instance()->GetOffset()
			, RB + ScreenShakeManager::Instance()->GetOffset(), Color(255, 0, 0, 255), true);
	}*/
}

void Player_Ultimate_01::Action_End()
{
	// 終了フラグを上げる
	End();
}
