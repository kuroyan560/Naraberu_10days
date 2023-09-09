#pragma once
#include "Common/Singleton.h"
#include "FrameWork/UsersInput.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "FrameWork/WinApp.h"
#include "../ExistUnits.h"
#include "../../RefreshRate.h"
#include"../../OperationConfig.h"

#include <vector>
#include <string>
#include <array>
#include <algorithm>

class BattleTurnMgr;
class UnitBase;

class Reticle : public KuroEngine::DesignPattern::Singleton<Reticle>
{
	friend class KuroEngine::DesignPattern::Singleton<Reticle>;

private:
	std::shared_ptr<KuroEngine::TextureBuffer> m_BigReticleTex;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 4> m_ReticleTex;

	struct Reticle_Aura {
		KuroEngine::Vec2<float> m_Position;
		UnitBase* UnitPtr = nullptr;
		std::vector<KuroEngine::Vec2<float>> Points;
		float m_Timer;
		bool m_Reverse;
		int Index;
		Reticle_Aura() {
			m_Position = KuroEngine::Vec2<float>(-100.0f, -100.0f);
			m_Timer = 0;
			Index = 0;
		}
		void SetPoint(UnitBase* ptr, bool Reverse);
		void Update();
		void Draw();
	};
	// 残留するエフェクト
	std::vector<Reticle_Aura> m_Aura;

	// レティクルアニメーション用
	float m_Reticle_Timer = 0;
	KuroEngine::Color MaskColor;

	// BattleTurnMgrのポインタ
	BattleTurnMgr* m_pBTM;

public:
	// 操作可能かどうか
	bool m_CanMove;

	Reticle();

	void SetBattleTurnManager(BattleTurnMgr* ptr);
	void Update();
	void Draw(int Index, KuroEngine::Vec2<float> LT, KuroEngine::Vec2<float> RB, KuroEngine::Color Mask);
	
	// 上のユニットが生きているか
	bool IsAliveUpperUnit();

	// 下のユニットが生きているか
	bool IsAliveUnderUnit();

	//void Move(int Index);
};