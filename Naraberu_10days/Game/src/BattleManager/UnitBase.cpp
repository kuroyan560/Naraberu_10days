#include "UnitBase.h"

void UnitBase::StartTurn()
{
	// ターンスタート
	m_isMyTurn = true;
	// タイマーリセット
	m_Frame_Timer = 0;
	// 
	m_IsEndTurnFunc = false;

	m_NextTurn = false;
}

void UnitBase::EndTurn()
{
	// ターン終了
	m_isMyTurn = false;
	// 即時ターン切替
	m_NextTurn = true;
}

void UnitBase::End()
{
	// ターン切り替えフラグを下す
	m_NextTurn = false;
}
