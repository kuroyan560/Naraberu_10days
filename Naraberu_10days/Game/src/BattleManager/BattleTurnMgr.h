#pragma once
#include "UnitBase.h"
#include <vector>
#include <memory>

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

#include "CutIn/CutInMgr.h"
#include "../RefreshRate.h"

class BattleTurnMgr
{
	std::vector<std::shared_ptr<UnitBase>> UnitList;
	// �S�̃^�[����
	int m_Whole_Turn_Count;
	// ���Ԗڂ̃��j�b�g�̃^�[����
	int TurnNum;
	// �^�[���̌o�߃t���[��
	int TurnFrameTime;
	// ���̃o�g���Ɉړ�����ꍇ�̃^�C�}�[
	int NextGameTimer;
	// ���̃o�g���Ɉړ�����ꍇ�̃^�C�}�[�̏I���l
	int NextGameTimeFinish;
	// ��̃^�C�}�[�̃f�t�H���g�l
	const int NextGameTimeFinish_Default = 300;

	// �摜
	std::shared_ptr<KuroEngine::TextureBuffer> m_CutInTex;

	// �s�k�t���O
	bool m_IsDefeat;

public:
	BattleTurnMgr() {
		m_Whole_Turn_Count = 0;
		TurnNum = 0;
		TurnFrameTime = 0;
		NextGameTimer = 0;
		NextGameTimeFinish = int(float(NextGameTimeFinish_Default) * RefreshRate::RefreshRate_Mag);
		m_IsDefeat = false;
	}

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
};

