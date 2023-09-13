#include "Debug_Jamming_01.h"
#include "../../Enemy/Enemy.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/WinApp.h"
#include "FrameWork/UsersInput.h"
#include"../../../SoundConfig.h"
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
		// パターンを決定
		int JamPattern = GetUnitPtr_nama<Enemy*>(m_Initiator)->m_Data.m_JammingPattern;
		// 
		// 桁
		int Digit = GetRand(0, 4);
		int pattern = KuroEngine::GetSpecifiedDigitNum(JamPattern, Digit, false);

		// ランダム生成 3個
		if (pattern == 0) {
			for (int i = 0; i < 3; i++) {
				Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
				ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			}
		}
		// ランダム生成 3個 プレイヤーに10ダメージ
		if (pattern == 1) {
			for (int i = 0; i < 3; i++) {
				Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
				ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			}
			ExistUnits::Instance()->m_pPlayer->Damage(10);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// ランダム生成 3個 プレイヤーに20ダメージ
		if (pattern == 2) {
			for (int i = 0; i < 3; i++) {
				Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 10), KuroEngine::GetRand(0, 10));
				ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			}
			ExistUnits::Instance()->m_pPlayer->Damage(20);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// 2 * 2 を生成
		else if (pattern == 3) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
		}
		// 3 * 3 で十字の穴あき を生成
		else if (pattern == 4) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 8), KuroEngine::GetRand(0, 8));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 2));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 2));
		}
		// 十字 を生成
		else if (pattern == 5) {
			Vec2<int> SetPos;
			// (1,1)等全消し不可が確定する配置はダメ
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(1, 9), KuroEngine::GetRand(1, 9));
				if (SetPos == Vec2(1, 1) || SetPos == Vec2(8, 8) || SetPos == Vec2(1, 8) || SetPos == Vec2(8, 1)) {
					continue;
				}
				else {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(-1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, -1));
		}
		// 十字 を生成 ＆ プレイヤーに40ダメージ
		else if (pattern == 6) {
			Vec2<int> SetPos;
			// (1,1)等全消し不可が確定する配置はダメ
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(1, 9), KuroEngine::GetRand(1, 9));
				if (SetPos == Vec2(1, 1) || SetPos == Vec2(8, 8) || SetPos == Vec2(1, 8) || SetPos == Vec2(8, 1)) {
					continue;
				}
				else {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(-1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, -1));
			ExistUnits::Instance()->m_pPlayer->Damage(40);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// 2 * 2 を 2個 生成
		else if (pattern == 7) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
			// 同じ位置は避ける
			Vec2 Old = SetPos;
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
				if (Old != SetPos) {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
		}
		// 2 * 2 を 2個 生成 ＆ プレイヤーに40ダメージ
		else if (pattern == 8) {
			Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
			// 同じ位置は避ける
			Vec2 Old = SetPos;
			while (1) {
				SetPos = Vec2<int>(KuroEngine::GetRand(0, 9), KuroEngine::GetRand(0, 9));
				if (Old != SetPos) {
					break;
				}
			}
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 1));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 0));
			ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(1, 1));
			ExistUnits::Instance()->m_pPlayer->Damage(40);
			SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}
		// 3 * 3 で十字の穴あき を生成 ＆ プレイヤーに70ダメージ
		else if (pattern == 9) {
		Vec2<int> SetPos = Vec2<int>(KuroEngine::GetRand(0, 8), KuroEngine::GetRand(0, 8));
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos);
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(0, 2));
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 0));
		ExistUnits::Instance()->m_StageManager->SetObstacle(SetPos + Vec2(2, 2));
		ExistUnits::Instance()->m_pPlayer->Damage(70);
		SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE);
		}

		SoundConfig::Instance()->Play(SoundConfig::SE_PUT_OJAMA);
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
