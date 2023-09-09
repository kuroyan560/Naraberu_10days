#include "SoundConfig.h"
#include"FrameWork/AudioApp.h"
#include"KuroEngine.h"

std::vector<int> SoundConfig::LoadSoundArray(std::string arg_dir, std::string arg_name, float arg_volume)
{
	using namespace KuroEngine;

	std::vector<int>result;
	int soundIdx = 0;
	while (ExistFile(arg_dir + arg_name + "_" + std::to_string(soundIdx) + ".wav"))
	{
		result.emplace_back(AudioApp::Instance()->LoadAudio(arg_dir + arg_name + "_" + std::to_string(soundIdx++) + ".wav", arg_volume));
	}

	return result;
}

SoundConfig::SoundConfig() : Debugger("SoundConfig")
{
	using namespace KuroEngine;

	std::fill(m_seEachVol.begin(), m_seEachVol.end(), 1.0f);
	std::fill(m_bgmEachVol.begin(), m_bgmEachVol.end(), 1.0f);
	
	//SEのファイル名設定
	std::array<std::string, SE_NUM>seFileName =
	{
		"select",
		"done",
		"cancel",

		"select",
		"cancel",
	};
	//BGMのファイル名指定
	std::array<std::string, BGM_NUM>bgmFileName =
	{
	};

	auto audioApp = AudioApp::Instance();
	std::string seDir = "resource/user/sound/";

	//SE読み込み
	m_seTable[SE_SELECT].Load(audioApp->LoadAudio(seDir + seFileName[SE_SELECT] + ".wav"));
	m_seTable[SE_DONE].Load(audioApp->LoadAudio(seDir + seFileName[SE_DONE] + ".wav"));
	m_seTable[SE_CANCEL].Load(audioApp->LoadAudio(seDir + seFileName[SE_CANCEL] + ".wav"));
	m_seTable[SE_MOVE_PRISM].Load(audioApp->LoadAudio(seDir + seFileName[SE_MOVE_PRISM] + ".wav"));
	m_seTable[SE_SELECT_PRISM].Load(audioApp->LoadAudio(seDir + seFileName[SE_SELECT_PRISM] + ".wav"));

	//m_seTable[SE_SURFACE_JUMP].Load(LoadSoundArray(seDir, seFileName[SE_SURFACE_JUMP]), SoundSE::ORDER_TYPE::RANDOM);
	//m_seTable[SE_LEVER_ON].Load(LoadSoundArray(seDir, seFileName[SE_LEVER_ON]), SoundSE::ORDER_TYPE::IN_ORDER);
	//BGM読み込み
	std::string bgmDir = "resource/user/sound/bgm/";

	//全部読み込んだか確認
	for (int seIdx = 0; seIdx < SE_NUM; ++seIdx)
	{
		if (!m_seTable[seIdx].m_invalid)continue;
		AppearMessageBox("SoundConfig コンストラクタ", "読み込まれていないSEがあるよ。");
		exit(1);
	}
	for (int bgmIdx = 0; bgmIdx < BGM_NUM; ++bgmIdx)
	{
		if (m_bgmTable[bgmIdx] != INVALID_SOUND)continue;
		AppearMessageBox("SoundConfig コンストラクタ", "読み込まれていないBGMがあるよ。");
		exit(1);
	}

	//カスタムパラメータより各音声の個別のボリュームを読み込み
	for (int seIdx = 0; seIdx < SE_NUM; ++seIdx)
	{
		AddCustomParameter(seFileName[seIdx], { "individualVolme","se",seFileName[seIdx] }, PARAM_TYPE::FLOAT, &m_seEachVol[seIdx], "IndividualVolme(SE)", true, 0.0f, 1.5f);
	}
	for (int bgmIdx = 0; bgmIdx < BGM_NUM; ++bgmIdx)
	{
		AddCustomParameter(bgmFileName[bgmIdx], { "individualVolme","bgm",bgmFileName[bgmIdx] }, PARAM_TYPE::FLOAT, &m_bgmEachVol[bgmIdx], "IndividualVolme(BGM)", true, 0.0f, 1.5f);
	}
	LoadParameterLog();

	//音量個別設定
	UpdateIndividualVolume();
}

int SoundConfig::SoundSE::GetPlaySoundHandle()
{
	int result = m_sounds[m_latestIdx];

	//複数ある場合次に再生するサウンドのインデックス更新
	if (1 < m_sounds.size())
	{
		switch (m_order)
		{
			//順列
			case SoundConfig::SoundSE::IN_ORDER:
				if (static_cast<int>(m_sounds.size()) <= ++m_latestIdx)m_latestIdx = 0;
				break;
			//ランダム
			case SoundConfig::SoundSE::RANDOM:
				do {
					m_latestIdx = KuroEngine::GetRand(static_cast<int>(m_sounds.size() - 1));
				} while (m_latestIdx == result);
				break;
			default:
				break;
		}
	}

	return result;
}

void SoundConfig::SoundSE::Play(int arg_delay, int arg_soundIdx)
{
	int soundIdx = arg_soundIdx == -1 ? GetPlaySoundHandle() : m_sounds[arg_soundIdx];
	arg_delay == -1 ? KuroEngine::AudioApp::Instance()->PlayWave(soundIdx) : KuroEngine::AudioApp::Instance()->PlayWaveDelay(arg_delay);
}

void SoundConfig::SoundSE::SetVolume(float arg_vol)
{
	for (auto& sound : m_sounds)
	{
		KuroEngine::AudioApp::Instance()->ChangeVolume(sound, arg_vol);
	}
}

void SoundConfig::UpdateIndividualVolume()
{
	for (int seIdx = 0; seIdx < SE_NUM; ++seIdx)
	{
		m_seTable[seIdx].SetVolume(m_seEachVol[seIdx]);
	}
	for (int bgmIdx = 0; bgmIdx < BGM_NUM; ++bgmIdx)
	{
		KuroEngine::AudioApp::Instance()->ChangeVolume(m_bgmTable[bgmIdx], m_bgmEachVol[bgmIdx]);
	}
}

void SoundConfig::Init()
{
	for (auto& se : m_seTable)se.Init();
}

void SoundConfig::Play(SE arg_se, int arg_delay, int arg_soundIdx)
{
	m_seTable[arg_se].Play(arg_delay, arg_soundIdx);
}

void SoundConfig::Play(BGM arg_bgm)
{
	//既に再生中ならそれを停止
	if (m_nowPlayBGMHandle != INVALID_SOUND)
	{
		KuroEngine::AudioApp::Instance()->StopWave(m_nowPlayBGMHandle);
	}
	KuroEngine::AudioApp::Instance()->PlayWave(m_bgmTable[arg_bgm], true);

	m_nowPlayBGMHandle = m_bgmTable[arg_bgm];
}