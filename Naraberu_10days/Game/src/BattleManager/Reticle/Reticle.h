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
	// �c������G�t�F�N�g
	std::vector<Reticle_Aura> m_Aura;

	// ���e�B�N���A�j���[�V�����p
	float m_Reticle_Timer = 0;
	KuroEngine::Color MaskColor;

	// BattleTurnMgr�̃|�C���^
	BattleTurnMgr* m_pBTM;

public:
	// ����\���ǂ���
	bool m_CanMove;

	Reticle();

	void SetBattleTurnManager(BattleTurnMgr* ptr);
	void Update();
	void Draw(int Index, KuroEngine::Vec2<float> LT, KuroEngine::Vec2<float> RB, KuroEngine::Color Mask);
	
	// ��̃��j�b�g�������Ă��邩
	bool IsAliveUpperUnit();

	// ���̃��j�b�g�������Ă��邩
	bool IsAliveUnderUnit();

	//void Move(int Index);
};