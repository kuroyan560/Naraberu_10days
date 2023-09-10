#pragma once
#include <memory>
#include "../RefreshRate.h"
class UnitBase
{
protected:
	// HP
	int m_HP;
	int m_MaxHP;

	// HPゲージが削れる演出用タイマー
	int m_HP_Break_Timer;
	// 被ダメor回復前のHP
	int m_Before_HP;
	// HPゲージが削れてく時間
	int m_HP_GAUGE_BREAK_TIME;

public:
	// 現在自分のターンか
	bool m_isMyTurn;
	// 次のターンに移行するフラグ
	bool m_NextTurn;
private:
	// 自ターンの経過フレームタイマー
	int m_Frame_Timer;

public:
	UnitBase(){
		m_HP = 100;
		m_MaxHP = 100;
		m_HP_Break_Timer = 0;
		m_Before_HP = 100;
		m_HP_GAUGE_BREAK_TIME = int(50.0f * RefreshRate::RefreshRate_Mag);
		m_isMyTurn = false;
		m_NextTurn = false;
		m_IsEndTurnFunc = false;
		m_Frame_Timer = 0;
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
	// ターン終了済みか
	bool m_IsEndTurnFunc;

	bool IsNextTurn() { return m_NextTurn; }

	// ダメージを受ける
	void Damage(int value) {
		// HPが削れていく演出用
		m_Before_HP = m_HP;
		m_HP_Break_Timer = 1;
		// ダメージ
		m_HP -= value;
		if (m_HP < 0) {
			m_HP = 0;
		}
	}

	// 回復する
	void Heal(int value) {
		// HPが回復する演出用
		m_Before_HP = m_HP;
		m_HP_Break_Timer = 1;
		// 回復
		m_HP += value;
		if (m_HP > m_MaxHP) {
			m_HP = m_MaxHP;
		}
	}

	// 回復する(割合)
	void Heal_Par(int value) {
		// HPが回復する演出用
		m_Before_HP = m_HP;
		m_HP_Break_Timer = 1;
		// 回復
		m_HP += int(float(m_MaxHP) * float(value) / 100.0f);
		if (m_HP > m_MaxHP) {
			m_HP = m_MaxHP;
		}
	}

	// 生きてればtrue
	bool IsAlive() { return m_HP > 0; };
};

/// <summary>
/// Unitbase*を UnitBase を継承した class型<T> にキャストする
/// </summary>
/// <typeparam name="T">キャストするclass名</typeparam>
/// <param name="Unit">キャストしたいUnitBase*オブジェクト</param>
/// <returns></returns>
template <class T>
static std::shared_ptr<T> GetUnitPtr(std::shared_ptr<UnitBase> Unit) { return std::dynamic_pointer_cast<T>(Unit); }

template <class T>
static T GetUnitPtr_nama(UnitBase* Unit) { return dynamic_cast<T>(Unit); }

// 使用例
// Unit.emplace_back(new Enemy);
// Enemy* EnemyPtr = GetUnitPtr<Enemy*>(Unit);