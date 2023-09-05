//#include "Debug_Heal_01.h"
//#include "Enemy.h"
//
//#include "../../src/engine/DirectX12/D3D12App.h"
//#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
//#include "../../src/engine/FrameWork/WinApp.h"
//#include "../../src/engine/FrameWork/UsersInput.h"
//
//void Debug_Heal_01::Param_Set(int Damage, int Duration)
//{
//	m_Damage = Damage;
//	m_Timer = 0;
//	m_Finish = Duration;
//}
//
//void Debug_Heal_01::Need_Object_Set(UnitBase* pUnit)
//{
//	// 自ユニットのポインタをセット
//	m_pUnit = pUnit;
//}
//
//void Debug_Heal_01::Action_Start()
//{
//	// 基底初期化
//	E_Ac_Base::Action_Start();
//}
//
//void Debug_Heal_01::Action_Update()
//{
//	// 終了処理
//	if (GetEndTimer()) {
//		Action_End();
//		return;
//	}
//
//	// タイマー加算
//	m_Timer++;
//
//
//}
//
//void Debug_Heal_01::Action_Draw()
//{
//	// 終了済みの場合は描画しない
//	if (GetEnd()) {
//		return;
//	}
//
//	using namespace KuroEngine;
//	// 仮で箱を描画
//	DrawFunc2D::DrawBox2D(Vec2(0.0f, 0.0f), Vec2(100.0f, 100.0f), Color(0, 255, 0, 255), true);
//}
//
//void Debug_Heal_01::Action_End()
//{
//	// 終了フラグを上げる
//	End();
//}
