#pragma once
#include "KuroEngineDevice.h"

#include "../BattleManager/Player/Player.h" 
#include "../BattleManager/Enemy/Enemy.h"
#include "../BattleManager/BattleTurnMgr.h"
#include "../Block/BlockManager.h"
#include "../Panel/PanelManager.h"
#include "../Stage/StageList.h"
#include "../Fade/Fade.h"
#include"../Effect/SetPrismEffect.h"
#include "../UI/PlayerDamageUi.h"

class ParticleEmitter;

class BattleScene :
    public KuroEngine::BaseScene
{
	// �v���C���[
	std::shared_ptr<Player> Pl;
	// �G���X�g
	std::vector<std::shared_ptr<UnitBase>> En;
	// �o�g��
	BattleTurnMgr Mgr;
	// �w�i
	std::shared_ptr<KuroEngine::TextureBuffer> m_SukasiTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_BackTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_StageTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_BattleTex;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,12> m_NumberTex;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 12> m_NumberTex_Battle;
	//�u���b�N
	std::shared_ptr<BlockManager> block;
	//�X�e�[�W
	std::shared_ptr<PanelManager> stage;

	BlockAttribute attribute;

	// �X�e�[�W�N���A
	std::shared_ptr<KuroEngine::TextureBuffer> m_ClearTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_MaxComboTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TotalTurnTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_KeyTex_Clear;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_ControllerTex_Clear;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DoneTex_Clear;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10> m_ClearNumberTex;

	// �Q�[���I�[�o�[
	// 0 : ���g���C 1 : �X�e�[�W�Z���N�g
	int m_GameOverSelectIndex;
	bool m_Already_Selected;
	std::shared_ptr<KuroEngine::TextureBuffer> m_GameoverTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_RetryTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_StageSelectTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_SelectTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_KeyTex_GameOver;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_ControllerTex_GameOver;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DoneTex_GameOver;

	// �t�F�[�h
	Fade m_Fade;

	std::shared_ptr<ParticleEmitter>m_healPtEmitter;
	std::shared_ptr<ParticleEmitter>m_backPrismPtEmitter;
	std::shared_ptr<ParticleEmitter>m_ultPtEmitter;

	std::shared_ptr<PlayerDamageUi> dame;

	void PlayerTurn();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

	// �v���Y����ݒu�o���Ȃ��悤�ɂ���^�C�}�[
	int m_Impossible_Put_Block_Timer;
	// �v���Y����ݒu�o���Ȃ��悤�ɂ��鎞��
	int m_Impossible_Put_Block_Effect_Time;

	// ���݂̃X�e�[�W
	Stages::StageData m_NowStage;
	// ���݂̃E�F�[�u
	int m_NowWave;
	// �X�e�[�W���Z�b�g
	void SetStage(std::string StageName);
	// ���̃E�F�[�u�X�^�[�g
	void NextWave();
	// �S�E�F�[�u�I��������
	bool m_Stage_End;
	// �X�e�[�W�I�����o�p
	int m_End_Timer;
	int m_End_Timer_Finish;

	// �Q�[���N���A�`��
	void GameClearDraw();
	// �Q�[���I�[�o�[���
	void GameOverDraw();
	// �|�[�Y���
	void PauseDraw();
	
	// ���U���g�\���p
	float ResultTimer;
	float ResultTimer_Max;
	float ResultEasing(float time);

	// �|�[�Y�p
	bool m_IsPause;
	// 0 : �ĊJ 1 : ���g���C 2 : �X�e�[�W�Z���N�g��
	int m_PauseMenu;
	bool m_Already_Selected_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_PauseTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_ResumeTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_RetryTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_StageSelectTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_SelectCosorTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_KeyTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_ControllerTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DoneTex_Pause;

	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Done;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Pass;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Left;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Right;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Set;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_TurnEnd;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Ult;

	//�v���C���[�U�����o
	std::shared_ptr<SetPrismEffect>m_playerAttackEffect;

public:
	BattleScene();

};

static DirectX::XMFLOAT3 hsv2rgb(DirectX::XMFLOAT3 hsv)
{
	DirectX::XMFLOAT3  rgb = XMFLOAT3();
	int     Hi;
	float   f;
	float   p;
	float   q;
	float   t;

	Hi = int(fmod(floor(hsv.x / 60.0f), 6.0f));
	f = hsv.x / 60.0f - Hi;
	p = hsv.z * (1.0f - hsv.y);
	q = hsv.z * (1.0f - f * hsv.y);
	t = hsv.z * (1.0f - (1.0f - f) * hsv.y);

	if (Hi == 0) {
		rgb.x = hsv.z;
		rgb.y = t;
		rgb.z = p;
	}
	if (Hi == 1) {
		rgb.x = q;
		rgb.y = hsv.z;
		rgb.z = p;
	}
	if (Hi == 2) {
		rgb.x = p;
		rgb.y = hsv.z;
		rgb.z = t;
	}
	if (Hi == 3) {
		rgb.x = p;
		rgb.y = q;
		rgb.z = hsv.z;
	}
	if (Hi == 4) {
		rgb.x = t;
		rgb.y = p;
		rgb.z = hsv.z;
	}
	if (Hi == 5) {
		rgb.x = hsv.z;
		rgb.y = p;
		rgb.z = q;
	}

	return rgb;
}