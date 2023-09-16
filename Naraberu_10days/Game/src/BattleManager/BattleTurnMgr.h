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
public:
	bool m_Selected_TurnEnd;
	bool m_Checked_TurnEnd;
private:
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

	//���ԃM���M���̎��̉��o�p
	bool gageBG;
	float gageBGTimer;

	// �s�k�t���O
	bool m_IsDefeat;


	// �^�[���G���h����
	SYSTEMTIME StartTime;
	SYSTEMTIME NowTime;
	__int64 m_ProgressTime;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Timer_Frame_Tex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Timer_Gauge_Tex;
	bool m_StandByTurnEnd;
public:
	int m_TimeUp_Eff_Timer;
	void DrawTimerCutIn();
private:
	int m_TimeUp_Eff_Timer_Max;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TimeUp_Eff_Timer_CutIn;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TimeUp_Eff_Timer_CutIn_Back;
	std::vector<float> T_C_Points;
	float OutQuint(float t) {
		return 1.0f - powf(1.0f - t, 5.0f);
	}
	float InQuint(float t) {
		return t * t * t * t * t;
	}
	float EaseFunc(float start, float end, float Now, float Max) {
		float t = Now / Max;
		float result = start + static_cast<float>(OutQuint(t) * static_cast<float>(end - start));
		return result;
	}
	float EaseFunc2(float start, float end, float Now, float Max) {
		float t = Now / Max;
		float result = start + static_cast<float>(InQuint(t) * static_cast<float>(end - start));
		return result;
	}
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

	// ���̃o�g���̍ŏ��̃^�[����
	bool m_FirstTurn;
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

	void DrawEnemy_HP_Gauge();

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

	//���ԃM���M���ɂȂ������̏���
	void JustInTime(const float _Now_Rate, const KuroEngine::Vec2<float> _pos1, const KuroEngine::Vec2<float> _pos2);
};

