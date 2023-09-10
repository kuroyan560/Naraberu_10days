#pragma once
#include "UnitBase.h"
#include <vector>
#include <memory>
#include <Windows.h>

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

#include "CutIn/CutInMgr.h"
#include "../RefreshRate.h"

class BattleTurnMgr
{
	// �S�̃^�[���� (�ʍ��Z)
	int m_Whole_Turn_Count;
	// �S�̃^�[���� (�ʍ��Z)
	int m_Total_Turn_Count;
	// ���Ԗڂ̃��j�b�g�̃^�[����
	int TurnNum;
	// �^�[���̌o�߃t���[��
	int TurnFrameTime;
	// ���̃o�g���Ɉړ�����ꍇ�̃^�C�}�[
	int NextGameTimer;
	// ���̃o�g���Ɉړ�����ꍇ�̃^�C�}�[�̏I���l
	int NextGameTimeFinish;
	// ��̃^�C�}�[�̃f�t�H���g�l
	const int NextGameTimeFinish_Default = 150;

	// �摜
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;
	// �^�[���G���h
	bool m_Selected_TurnEnd;
	bool m_Checked_TurnEnd;
	int m_Selected_TurnEnd_Timer;
	float m_Scaling_Timer;
	float m_Scaling_Timer_Max;
	bool m_Moving_Flag;
	float m_Moving_Timer;
	float m_Moving_Timer_Max;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEndTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEnd_EnterTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEnd_Crtl_EnterTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TurnEnd_SelectedTex;
	void TurnEndButtonUpdate();
	void TurnEndButtonDraw();
	float ResultEasing(float time);

	// �s�k�t���O
	bool m_IsDefeat;

	// �^�[���G���h����
	SYSTEMTIME StartTime;
	SYSTEMTIME NowTime;
	__int64 m_ProgressTime;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Timer_Frame_Tex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Timer_Gauge_Tex;
public:
	SYSTEMTIME PauseStartTime;
	SYSTEMTIME PauseEndTime;
	__int64 m_PauseTime;
	std::vector<__int64> m_PauseTimeContainer;

public:
	std::vector<std::shared_ptr<UnitBase>> UnitList;

	BattleTurnMgr();

	void OnInitialize(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);
	// �E�F�[�u�ɊJ�n���Ƀ��j�b�g���Z�b�g
	void SetUnits(std::shared_ptr<UnitBase> Player, std::vector<std::shared_ptr<UnitBase>> Enemys);

	void OnUpdate();
	void OnDraw();
	void OnImguiDebug();

	void NextTurnStart();

	// ���̃o�g���ɍs���ꍇ�̍X�V����
	void Update_NextWave();
	// �v���C���[�ƓG�������Ă���ꍇ�̍X�V����
	void Update_Battle();

	// ���̃E�F�[�u�Ɉړ����邩
	bool ChangeNextWave() { return NextGameTimer >= NextGameTimeFinish; }

	// �����Ă���G�����邩
	bool AliveEnemys();

	// �v���C���[������ł���A�s�k���m�肵�Ă��邩
	bool GetDefeat() { return m_IsDefeat; }

	// ���v�^�[���擾
	int GetTotalTurn() { return m_Total_Turn_Count; }

	// �^�[���G���h�{�^���I�𒆂�
	bool GetSelectedTurnEnd() { return m_Selected_TurnEnd; }

	// ���ԃQ�[�W�`��
	void AutoTurnEndTimerDraw();

	// ���݃^�[���擾
	int GetNowTurn() { return TurnNum; }
};

