#pragma once
#include <vector>
#include <memory>

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

#include "Common/Singleton.h"

#include "../../RefreshRate.h"

enum class CutInType {
	PLAYER_TURN,
	ENEMY_TURN,
	NEXT_BATTLE,
	MAX
};

class CutInMgr : public KuroEngine::DesignPattern::Singleton<CutInMgr>
{
	friend class KuroEngine::DesignPattern::Singleton<CutInMgr>;

	int CutInTimer;
	int CutInEndTime = 200;
	float inQuart(float time, float begin, float change, float duration) {
		return change * powf((time / duration), 4) + begin;
	}
	float outQuart(float time, float begin, float change, float duration) {
		return -change * (powf((time / duration - 1), 4) - 1) + begin;
	}
	float OutInQuart(float time, float begin, float change, float duration) {
		if (time / duration < 0.5f) {
			return outQuart(time * 2, begin, change / 2, duration);
		}
		return inQuart((time * 2) - duration, begin + change / 2, change / 2, duration);
	}


	// ‰æ‘œ
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;

	// 
	struct CutInData {
		CutInType m_Type;
		int m_CutInEndTime;
		std::vector<std::shared_ptr<KuroEngine::TextureBuffer>> m_Tex;

		template<class... A>
		CutInData(CutInType Type, int CutInEndTime, A... TexPathes) {
			m_Type = Type;
			m_CutInEndTime = int(float(CutInEndTime) * float(RefreshRate::RefreshRate_Mag));
			for (std::string TexPath : std::initializer_list<std::string>{ TexPathes... }) {
				m_Tex.emplace_back(KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexPath));
			}
		}
	};

	std::shared_ptr<CutInData> NowCutInPtr;

	CutInType m_NowCutInType;

public:
	void OnInitialize();
	void StartCutIn(CutInType Type);

	void OnUpdate();
	void OnDraw();

	bool NowCutIn() { return CutInTimer > 0 ? true : false; }
};

