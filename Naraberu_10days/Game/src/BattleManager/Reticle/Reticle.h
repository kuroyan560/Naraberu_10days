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

	/// <summary>
	/// ���T�[�W���Ȑ�
	/// </summary>
	/// <param name="a">�p���g�� a</param>
	/// <param name="b">�p���g�� b</param>
	/// <param name="fi">�ʑ��� fi</param>
	/// <param name="t">�lt</param>
	/// <returns></returns>
	KuroEngine::Vec2<float> Lissajou(float a, float b, float fi, float t) {
		KuroEngine::Vec2<float> result;
		result.x = sin((a * t) + fi);
		result.y = sin(b * t);
		return result;
	}

	// 2�̓_�̋������v�Z����֐�
	float distance(const KuroEngine::Vec2<float>& p1, const KuroEngine::Vec2<float>& p2) {
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// �_�Ԃ𓙑��ňړ�����֐�
	KuroEngine::Vec2<float> getCurrentPosition(std::vector<KuroEngine::Vec2<float>>& points, int framesPerCycle, int* currentIndex, int currentFrame) {
		int n = int(points.size());
		int nextPointIndex = (*currentIndex + 1) % n; // ���̓_�̃C���f�b�N�X
		float dist = distance(points[*currentIndex], points[nextPointIndex]); // ���݂̓_���玟�̓_�܂ł̋���
		float totalTime = float(framesPerCycle); // ����ɂ����鍇�v�̃t���[����
		float currentTime = float(currentFrame % framesPerCycle); // ���݂̃t���[�������������Ɏ��߂�
		float fraction = currentTime / totalTime; // ���݂̐i���i0.0����1.0�͈̔́j

		// ���݂̈ʒu���v�Z
		float currentX = points[*currentIndex].x + fraction * (points[nextPointIndex].x - points[*currentIndex].x);
		float currentY = points[*currentIndex].y + fraction * (points[nextPointIndex].y - points[*currentIndex].y);

		if (currentFrame % framesPerCycle == framesPerCycle - 1) {
			*currentIndex = (*currentIndex + 1) % int(points.size()); // ���̓_�Ɉړ�
		}

		return { currentX, currentY };
	}

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