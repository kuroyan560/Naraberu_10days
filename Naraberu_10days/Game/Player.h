#pragma once
#include "UnitBase.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "../../src/engine/FrameWork/WinApp.h"

#include "RefreshRate.h"

class Player :
    public UnitBase
{
private:

	// �摜
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;

	//�L�����摜
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterTex;

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

	// �v���C���[�^�[���I���O����
	void TurnEnd_BeforeTurnChange();

	// �S�X�e�[�^�X���Z�b�g����
	void SetState(int HP, int MaxHP);

	// �A���e�B���b�g�֘A
private:
	// ���݂̃A���e�B���b�g�|�C���g�̒l
	int m_UltimatePoint;
	// �ω��O�̃A���e�B���b�g�|�C���g
	int m_BeforeUltimatePoint;
	// �A���e�B���b�g�|�C���g�̍ő�l
	int Max_UltimatePoint;

public:
	// �A���e�B���b�g�|�C���g�����Z����
	void AddUltPoint(int Amount) {
		// �ω��O�̒l��ۑ�
		m_BeforeUltimatePoint = m_UltimatePoint;
		// �A���e�B���b�g�|�C���g�����Z
		m_UltimatePoint += Amount;
		m_UltimatePoint > Max_UltimatePoint ? m_UltimatePoint = Max_UltimatePoint : 0;
	}
	// ���݂̃A���e�B���b�g�|�C���g�̎擾
	int GetUltPoint() { return m_UltimatePoint; }
	// ���݂̃A���e�B���b�g�̃`���[�W�����擾
	float GetUltRate() { return float(m_UltimatePoint) / float(Max_UltimatePoint); }
	// �A���e�B���b�g�����܂��Ă��邩�ǂ���
	bool IsReadyUltimate() { return m_UltimatePoint >= Max_UltimatePoint; }

	// �`��ʒu�֘A
private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;
public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }
};

