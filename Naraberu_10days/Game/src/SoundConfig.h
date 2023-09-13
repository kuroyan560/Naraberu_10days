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
	//SE（再生機能のみ）
	enum SE
	{
		SE_SELECT,	//選択
		SE_DONE,	//決定
		SE_CANCEL,	//キャンセル

		SE_MOVE_PRISM,	//プリズム設置位置移動
		SE_SELECT_PRISM,	//プリズム選択

		SE_HEAL,
		SE_DAMAGE,	//敵にダメージ
		SE_PERFECT,
		SE_PERFECT_BONUS_DAMAGE,
		SE_CHARGE_ULT,	//ULTのチャージ
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
	//BGM（ループ再生あり）
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
		//読み込んだ音声のハンドル配列
		std::vector<int>m_sounds;
		//最後に再生した音声のインデックス
		int m_latestIdx = 0;
		//複数ある場合の再生順
		ORDER_TYPE m_order = IN_ORDER;
		//順列だった場合ループさせるか
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

	//各音声の個別のボリューム
	std::array<float, SE_NUM>m_seEachVol;
	std::array<float, BGM_NUM>m_bgmEachVol;

	//BGMのフェード用（０でメイン、１でサブ）
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

