#pragma once

class UnitBase
{
protected:
	// 現在自分のターンか
	bool m_isMyTurn;
	// 次のターンに移行するフラグ
	bool m_NextTurn;
public:
	UnitBase(){
		m_isMyTurn = false;
		m_NextTurn = false;
	};

	virtual void OnInitialize() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnImguiDebug() = 0;
	virtual void OnFinalize() = 0;

	void StartTurn();
	void EndTurn();

	void End();

	bool IsNextTurn() { return m_NextTurn; }

};

/// <summary>
/// Unitbase*を UnitBase を継承した class型<T> にキャストする
/// </summary>
/// <typeparam name="T">キャストするclass名</typeparam>
/// <param name="Unit">キャストしたいUnitBase*オブジェクト</param>
/// <returns></returns>
template <class T>
static T GetUnitPtr(UnitBase* Unit) { return dynamic_cast<T>(Unit); }
// 使用例
// Unit.emplace_back(new Enemy);
// Enemy* EnemyPtr = GetUnitPtr<Enemy*>(Unit);