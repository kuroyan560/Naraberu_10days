#pragma once
#include <array>
#include "../UnitBase.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "FrameWork/WinApp.h"

#include "../../RefreshRate.h"

#include<memory>
class UltActivateEffect;

class Player :
    public UnitBase
{
private:

	// �摜
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_break;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Ult_Gauge;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 12> m_NumberTex;

	//�L�����摜
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterDamageTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterMabatakiTex;
	// �܂΂����p�^�C�}�[
	int m_Player_Mabataki_Timer;
public:
	int TurnChangeTimer;
	int TurnChangeTime_Fin;

public:
	Player();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnAlwaysUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
private:
	int m_Damage_Timer;
public:
	// �_���[�W���󂯂�
	void Damage(int value)override;

	// �v���C���[�^�[���I���O����
	void TurnEnd_BeforeTurnChange();

	// �S�X�e�[�^�X���Z�b�g����
	void SetState(int HP, int MaxHP);

	// �E���g�����܂������̉��o
	void UltMaxEffect();

	// �^�[���G���h
	bool m_IsEndBonus;
	bool m_DoBonus;
	void TurnEndTrigger();

	// �A���e�B���b�g�֘A
private:
	// ���݂̃A���e�B���b�g�|�C���g�̒l
	int m_UltimatePoint;
	// �ω��O�̃A���e�B���b�g�|�C���g
	int m_BeforeUltimatePoint;
	// �A���e�B���b�g�|�C���g�̍ő�l
	int Max_UltimatePoint;

	//�A���e�B���b�g�̔������o
	std::weak_ptr<UltActivateEffect>m_UltimateActivateEffect;

public:
	// �E���g���o�̃|�C���^
	std::shared_ptr<UltActivateEffect> GetUltEffectPtr() { return m_UltimateActivateEffect.lock(); }
	// �A���e�B���b�g�|�C���g�����Z����
	void AddUltPoint(int Amount) {
		// �ω��O�̒l��ۑ�
		m_BeforeUltimatePoint = m_UltimatePoint;
		// �A���e�B���b�g�|�C���g�����Z
		m_UltimatePoint += Amount;
		m_UltimatePoint > Max_UltimatePoint ? m_UltimatePoint = Max_UltimatePoint : 0;
	}
	// �A���e�B���b�g�|�C���g�����Z����
	void SubUltPoint(int Amount) {
		// �ω��O�̒l��ۑ�
		m_BeforeUltimatePoint = m_UltimatePoint;
		// �A���e�B���b�g�|�C���g�����Z
		m_UltimatePoint -= Amount;
		m_UltimatePoint < 0 ? m_UltimatePoint = 0 : 0;
	}
	// ���݂̃A���e�B���b�g�|�C���g�̎擾
	int GetUltPoint() { return m_UltimatePoint; }
	// ���݂̃A���e�B���b�g�̃`���[�W�����擾
	float GetUltRate() { return float(m_UltimatePoint) / float(Max_UltimatePoint); }
	// �A���e�B���b�g�����܂��Ă��邩�ǂ���
	bool IsReadyUltimate() { return m_UltimatePoint >= Max_UltimatePoint; }

	void EndBonusAttack() { m_IsEndBonus = true; }
	bool GetEndBonusAttack() { return m_IsEndBonus; }

	void SetUltActivateEffect(std::shared_ptr<UltActivateEffect>Effect) { m_UltimateActivateEffect = Effect; }

	// �`��ʒu�֘A
private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;

	std::array<KuroEngine::Vec2<float>,4> ultSize;


public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }
};

