#pragma once
#include "KuroEngineDevice.h"

class Fade : public KuroEngine::SceneTransition {

	bool FadeMode;
	int m_Timer;
	int m_FinishTime;
	double OutExpo(double t) {
		return t == 1.0 ? 1.0 : 1.0 - pow(2.0, -10.0 * t);
	}

	void OnStart()override;
public:
	//シーンを切り替えるタイミングでtrueを返す
	bool Update()override;
	//シーン遷移描画
	void Draw()override;

};