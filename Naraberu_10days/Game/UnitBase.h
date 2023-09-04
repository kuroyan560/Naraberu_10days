#pragma once

class UnitBase
{
protected:
	// ���ݎ����̃^�[����
	bool m_isMyTurn;
	// ���̃^�[���Ɉڍs����t���O
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
/// Unitbase*�� UnitBase ���p������ class�^<T> �ɃL���X�g����
/// </summary>
/// <typeparam name="T">�L���X�g����class��</typeparam>
/// <param name="Unit">�L���X�g������UnitBase*�I�u�W�F�N�g</param>
/// <returns></returns>
template <class T>
static T GetUnitPtr(UnitBase* Unit) { return dynamic_cast<T>(Unit); }
// �g�p��
// Unit.emplace_back(new Enemy);
// Enemy* EnemyPtr = GetUnitPtr<Enemy*>(Unit);