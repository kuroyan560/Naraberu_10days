#pragma once
#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include"ForUser/Debugger.h"
#include"Common/Singleton.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

namespace KuroEngine
{
	class Fps : public Debugger, public DesignPattern::Singleton<Fps>
	{
		friend class DesignPattern::Singleton<Fps>;
		Fps() :Debugger("Fps") {}

		int m_frameRate;
		float m_minFrameTime;
		float m_frameTime = 0;
		LARGE_INTEGER m_timeStart;
		LARGE_INTEGER m_timeEnd;
		LARGE_INTEGER m_timeFreq;
		// fps���擾����Ȃ�0�ŏ��������Ȃ��ƃS�~���������ă}�C�i�X����n�܂����肷��(�����m��Ȃ�)
		float m_fps = 0;

	public:
		//���C�����[�v�O�ɌĂяo��
		void LoopInit(const int& FrameRate);
		void Update();

		const float& GetNowFps()const { return m_fps; }
		void OnImguiItems()override;
	};
}