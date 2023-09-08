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

	/// <summary>
	/// リサージュ曲線
	/// </summary>
	/// <param name="a">角周波数 a</param>
	/// <param name="b">角周波数 b</param>
	/// <param name="fi">位相差 fi</param>
	/// <param name="t">値t</param>
	/// <returns></returns>
	KuroEngine::Vec2<float> Lissajou(float a, float b, float fi, float t) {
		KuroEngine::Vec2<float> result;
		result.x = sin((a * t) + fi);
		result.y = sin(b * t);
		return result;
	}

	// 2つの点の距離を計算する関数
	float distance(const KuroEngine::Vec2<float>& p1, const KuroEngine::Vec2<float>& p2) {
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// 点間を等速で移動する関数
	KuroEngine::Vec2<float> getCurrentPosition(std::vector<KuroEngine::Vec2<float>>& points, int framesPerCycle, int* currentIndex, int currentFrame) {
		int n = int(points.size());
		int nextPointIndex = (*currentIndex + 1) % n; // 次の点のインデックス
		float dist = distance(points[*currentIndex], points[nextPointIndex]); // 現在の点から次の点までの距離
		float totalTime = float(framesPerCycle); // 一周にかかる合計のフレーム数
		float currentTime = float(currentFrame % framesPerCycle); // 現在のフレーム数を周期内に収める
		float fraction = currentTime / totalTime; // 現在の進捗（0.0から1.0の範囲）

		// 現在の位置を計算
		float currentX = points[*currentIndex].x + fraction * (points[nextPointIndex].x - points[*currentIndex].x);
		float currentY = points[*currentIndex].y + fraction * (points[nextPointIndex].y - points[*currentIndex].y);

		if (currentFrame % framesPerCycle == framesPerCycle - 1) {
			*currentIndex = (*currentIndex + 1) % int(points.size()); // 次の点に移動
		}

		return { currentX, currentY };
	}

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