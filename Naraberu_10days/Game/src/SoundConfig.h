#pragma once
#include"Common/Singleton.h"
#include<vector>
#include<array>
#include<string>
#include"ForUser/Debugger.h"
#include"ForUser/Timer.h"

class SoundConfig : public KuroEngine::DesignPattern::Singleton<SoundConfig>, public KuroEngine::Debugger
{
public:
	//SE�i�Đ��@�\�̂݁j
	enum SE
	{
		SE_SELECT,	//�I��
		SE_DONE,	//����
		SE_CANCEL,	//�L�����Z��

		SE_MOVE_PRISM,	//�v���Y���ݒu�ʒu�ړ�
		SE_SELECT_PRISM,	//�v���Y���I��

		SE_HEAL,
		SE_DAMAGE,	//�G�Ƀ_���[�W
		SE_PERFECT,
		SE_PERFECT_BONUS_DAMAGE,
		SE_CHARGE_ULT,	//ULT�̃`���[�W
		SE_ACTIVATE_ULT,
		SE_BONUS_ATTACK_COUNT,

		SE_PUT_OJAMA,
		SE_PUT_GOLD,

		SE_STAGE_CLEAR,
		SE_GAME_OVER,

		SE_CANNOT_SELECT,
		SE_PAUSE_OPEN,
		SE_PAUSE_CLOSE,

		SE_NUM
	};
	//BGM�i���[�v�Đ�����j
	enum BGM
	{
		BGM_MAIN,
		BGM_SUB,
		BGM_NUM,
		BGM_NONE,
	};

private:
	friend class KuroEngine::DesignPattern::Singleton<SoundConfig>;
	SoundConfig();
	std::vector<int>LoadSoundArray(std::string arg_dir, std::string arg_name, float arg_volume = 1.0f);

	class SoundSE
	{
	public:
		enum ORDER_TYPE { IN_ORDER, RANDOM };
	private:
		//�ǂݍ��񂾉����̃n���h���z��
		std::vector<int>m_sounds;
		//�Ō�ɍĐ����������̃C���f�b�N�X
		int m_latestIdx = 0;
		//��������ꍇ�̍Đ���
		ORDER_TYPE m_order = IN_ORDER;
		//���񂾂����ꍇ���[�v�����邩
		bool m_inOrderLoop = false;
		
		int GetPlaySoundHandle();

	public:
		bool m_invalid = true;
		SoundSE() {}

		void Load(int arg_sound)
		{
			m_sounds = { arg_sound };
			m_invalid = false;
		}
		void Load(std::vector<int>arg_sounds, ORDER_TYPE arg_order, bool arg_inOrderLoop = false)
		{
			m_sounds = arg_sounds;
			m_order = arg_order;
			m_inOrderLoop = arg_inOrderLoop;
			m_invalid = false;
		}

		void Init()
		{
			m_latestIdx = 0;
		}
		void Play(int arg_delay = -1, int arg_soundIdx = -1,bool arg_resetLatestIdx = false);
		void SetVolume(float arg_vol);
	};

	static const int INVALID_SOUND = -1;
	std::array<SoundSE, SE_NUM>m_seTable;
	std::array<int, BGM_NUM>m_bgmTable = { INVALID_SOUND };

	//�e�����̌ʂ̃{�����[��
	std::array<float, SE_NUM>m_seEachVol;
	std::array<float, BGM_NUM>m_bgmEachVol;

	//BGM�̃t�F�[�h�p�i�O�Ń��C���A�P�ŃT�u�j
	BGM m_nowBgm = BGM_SUB;
	BGM m_nextBgm = BGM_NONE;
	KuroEngine::Timer m_bgmSwitchTimer;

	void OnImguiItems();

public:
	void Init();
	void Update();

	void Play(SE arg_se, int arg_delay = -1, int arg_soundIdx = -1, bool arg_resetOrderSound = false);
	void SwitchBGM(BGM arg_bgm);

	void UpdateIndividualVolume();
};

