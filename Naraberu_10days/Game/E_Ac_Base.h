#pragma once

// “Gs“®Šî’êƒNƒ‰ƒX
class E_Ac_Base
{	
protected:
	E_Ac_Base(){};
	int m_Timer = 0;
	int m_Finish = 1;
	bool m_isEnd = false;
public:
	virtual void Action_Start(){
		m_Timer = 0;
		m_isEnd = false;
	}
	virtual void Action_Update() = 0;
	virtual void Action_Draw() = 0;
	virtual void Action_End() = 0;

	bool GetEndTimer() { return m_Timer == m_Finish; }

	bool AlreadyStart() { return m_Timer > 0; }
	void End() { m_isEnd = true; };
	bool GetEnd() { return m_isEnd; }
};

