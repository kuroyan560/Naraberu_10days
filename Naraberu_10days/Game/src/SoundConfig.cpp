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

SoundConfig::SoundConfig() : Debugger("SoundConfig", false)
{
	using namespace KuroEngine;

	//BGM�t�F�[�h�����܂ł̃t���[����
	m_bgmSwitchTimer.Reset(100.0f);

	std::fill(m_seEachVol.begin(), m_seEachVol.end(), 1.0f);
	std::fill(m_bgmEachVol.begin(), m_bgmEachVol.end(), 1.0f);
	
	//SE�̃t�@�C�����ݒ�
	std::array<std::string, SE_NUM>seFileName =
	{
		"select",
		"done",
		"cancel",

		"select",
		"cancel",
	};
	//BGM�̃t�@�C�����w��
	std::array<std::string, BGM_NUM>bgmFileName =
	{
		"bgm_main",
		"bgm_sub",
	};

	auto audioApp = AudioApp::Instance();
	std::string seDir = "resource/user/sound/";

	//SE�ǂݍ���
	m_seTable[SE_SELECT].Load(audioApp->LoadAudio(seDir + seFileName[SE_SELECT] + ".wav"));
	m_seTable[SE_DONE].Load(audioApp->LoadAudio(seDir + seFileName[SE_DONE] + ".wav"));
	m_seTable[SE_CANCEL].Load(audioApp->LoadAudio(seDir + seFileName[SE_CANCEL] + ".wav"));
	m_seTable[SE_MOVE_PRISM].Load(audioApp->LoadAudio(seDir + seFileName[SE_MOVE_PRISM] + ".wav"));
	m_seTable[SE_SELECT_PRISM].Load(audioApp->LoadAudio(seDir + seFileName[SE_SELECT_PRISM] + ".wav"));

	//m_seTable[SE_SURFACE_JUMP].Load(LoadSoundArray(seDir, seFileName[SE_SURFACE_JUMP]), SoundSE::ORDER_TYPE::RANDOM);
	//m_seTable[SE_LEVER_ON].Load(LoadSoundArray(seDir, seFileName[SE_LEVER_ON]), SoundSE::ORDER_TYPE::IN_ORDER);
	
	//BGM�ǂݍ���
	std::string bgmDir = "resource/user/sound/";
	m_bgmTable[BGM_MAIN] = audioApp->LoadAudio(bgmDir + bgmFileName[BGM_MAIN] + ".wav");
	m_bgmTable[BGM_SUB] = audioApp->LoadAudio(bgmDir + bgmFileName[BGM_SUB] + ".wav");

	//�S���ǂݍ��񂾂��m�F
	for (int seIdx = 0; seIdx < SE_NUM; ++seIdx)
	{
		if (!m_seTable[seIdx].m_invalid)continue;
		AppearMessageBox("SoundConfig �R���X�g���N�^", "�ǂݍ��܂�Ă��Ȃ�SE�������B");
		exit(1);
	}
	for (int bgmIdx = 0; bgmIdx < BGM_NUM; ++bgmIdx)
	{
		if (m_bgmTable[bgmIdx] != INVALID_SOUND)continue;
		AppearMessageBox("SoundConfig �R���X�g���N�^", "�ǂݍ��܂�Ă��Ȃ�BGM�������B");
		exit(1);
	}

	//�J�X�^���p�����[�^���e�����̌ʂ̃{�����[����ǂݍ���
	for (int seIdx = 0; seIdx < SE_NUM; ++seIdx)
	{
		AddCustomParameter(seFileName[seIdx], { "individualVolme","se",seFileName[seIdx] }, PARAM_TYPE::FLOAT, &m_seEachVol[seIdx], "IndividualVolme(SE)", true, 0.0f, 1.5f);
	}
	for (int bgmIdx = 0; bgmIdx < BGM_NUM; ++bgmIdx)
	{
		AddCustomParameter(bgmFileName[bgmIdx], { "individualVolme","bgm",bgmFileName[bgmIdx] }, PARAM_TYPE::FLOAT, &m_bgmEachVol[bgmIdx], "IndividualVolme(BGM)", true, 0.0f, 1.5f);
	}
	LoadParameterLog();

	//���ʌʐݒ�
	UpdateIndividualVolume();

	KuroEngine::AudioApp::Instance()->ChangeVolume(m_bgmTable[BGM_MAIN], 0.0f);
	KuroEngine::AudioApp::Instance()->PlayWave(m_bgmTable[BGM_MAIN], true);
	KuroEngine::AudioApp::Instance()->PlayWave(m_bgmTable[BGM_SUB], true);
}

int SoundConfig::SoundSE::GetPlaySoundHandle()
{
	int result = m_sounds[m_latestIdx];

	//��������ꍇ���ɍĐ�����T�E���h�̃C���f�b�N�X�X�V
	if (1 < m_sounds.size())
	{
		switch (m_order)
		{
			//����
			case SoundConfig::SoundSE::IN_ORDER:
				if (static_cast<int>(m_sounds.size()) <= ++m_latestIdx)m_latestIdx = 0;
				break;
			//�����_��
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

void SoundConfig::OnImguiItems()
{
	if (CustomParamDirty())
	{
		for (int seIdx = 0; seIdx < SE_NUM; ++seIdx)
		{
			m_seTable[seIdx].SetVolume(m_seEachVol[seIdx]);
		}

		KuroEngine::AudioApp::Instance()->ChangeVolume(m_bgmTable[m_nowBgm], m_bgmEachVol[m_nowBgm]);
	}
}

void SoundConfig::Init()
{
	for (auto& se : m_seTable)se.Init();
}

void SoundConfig::Update()
{
	//BGM�̃t�F�[�h
	if (m_nextBgm != BGM_NONE)
	{
		m_bgmSwitchTimer.UpdateTimer();

		KuroEngine::AudioApp::Instance()->ChangeVolume(
			m_bgmTable[m_nowBgm], (1.0f - m_bgmSwitchTimer.GetTimeRate()) * m_bgmEachVol[m_nowBgm]);
		KuroEngine::AudioApp::Instance()->ChangeVolume(
			m_bgmTable[m_nextBgm], m_bgmSwitchTimer.GetTimeRate() * m_bgmEachVol[m_nextBgm]);

		if (m_bgmSwitchTimer.IsTimeUpOnTrigger())
		{
			m_nowBgm = m_nextBgm;
			m_nextBgm = BGM_NONE;
		}
	}
}

void SoundConfig::Play(SE arg_se, int arg_delay, int arg_soundIdx)
{
	m_seTable[arg_se].Play(arg_delay, arg_soundIdx);
}

void SoundConfig::SwitchBGM(BGM arg_bgm)
{
	if (m_nowBgm == arg_bgm)return;
	if (m_nextBgm != BGM_NONE)return;
	
	m_nextBgm = arg_bgm;
	m_bgmSwitchTimer.Reset();
}
