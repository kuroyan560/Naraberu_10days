#pragma once
#include <memory>

class UnitBase
{
protected:
	// 
	int m_HP = 200;
	int m_MaxHP = 200;

	// HPゲージが削れる演出用タイマー
	int m_HP_Break_Timer = 0;
	// 被ダメor回復前のHP
	int m_Before_HP = 200;
	// HPゲージが削れてく時間
	static const int m_HP_GAUGE_BREAK_TIME = 5;

	// 現在自分のターンか
	bool m_isMyTurn;
	// 次のターンに移行するフラグ
	bool m_NextTurn;
	// 自ターンの経過フレームタイマー
	int m_Frame_Timer;
public:
	UnitBase(){
		m_isMyTurn = false;
		m_NextTurn = false;
	};

	virtual void OnInitialize() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnAlwaysUpdate() = 0;
	virtual void OnDraw() = 0;
	virtual void OnImguiDebug() = 0;
	virtual void OnFinalize() = 0;

	void StartTurn();
	void EndTurn();

	void TimerUpdate() {
		m_Frame_Timer++;
	}

	void End();

	bool IsNextTurn() { return m_NextTurn; }

	void Damage(int value) {
		// HPが削れていく演出用
		m_Before_HP = m_HP;
		m_HP_Break_Timer = 1;

		m_HP -= value;
		if (m_HP < 0) {
			m_HP = 0;
		}
	}
};

/// <summary>
/// Unitbase*を UnitBase を継承した class型<T> にキャストする
/// </summary>
/// <typeparam name="T">キャストするclass名</typeparam>
/// <param name="Unit">キャストしたいUnitBase*オブジェクト</param>
/// <returns></returns>
template <class T>
static std::shared_ptr<T> GetUnitPtr(std::shared_ptr<UnitBase> Unit) { return std::dynamic_pointer_cast<T>(Unit); }
// 使用例
// Unit.emplace_back(new Enemy);
// Enemy* EnemyPtr = GetUnitPtr<Enemy*>(Unit);