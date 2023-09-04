#include "UnitBase.h"

void UnitBase::StartTurn()
{
	// ターンスタート
	m_isMyTurn = true;
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
