#include<windows.h>
#include"KuroEngineDevice.h"
#include"Common/Transform.h"
#include"Common/Transform2D.h"
#include"Common/Color.h"

#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Shadow.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_FillTex.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D_Color.h"

#include"ForUser/DrawFunc/3D/DrawFunc3D.h"

#include"ForUser/DrawFunc/BillBoard/DrawFuncBillBoard.h"

#include"ForUser/Debugger.h"

#include "../src/Scene/GameScene.h"
#include "../src/Scene/BattleScene.h"
#include "../src/Scene/TitleScene.h"
#include "../src/SoundConfig.h"

#ifdef _DEBUG
int main()
#else
//Windows�A�v���ł̃G���g���[�|�C���g�imain�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	//�G���W���ݒ�i���[�U�[�ݒ�j========
	KuroEngine::EngineOption engineOption;
	engineOption.m_windowName = "�}�W�J�v���Y��";
	engineOption.m_windowSize = { 1280,720 };
	//engineOption.m_windowSize = { 1920,1080 };
	engineOption.m_fullScreen = false;
	engineOption.m_showCursor = true;
	engineOption.m_clipCursor = false;
	engineOption.m_iconPath = L"resource/user/tex/icon.ico";
	engineOption.m_backBuffClearColor = KuroEngine::Color(50, 49, 59, 255);
	engineOption.m_useHDR = false;
	engineOption.m_frameRate = 120;

	//================================

	//�G���W���N���i���s������I���j
	KuroEngine::KuroEngineDevice engine;
	if (engine.Initialize(engineOption))return 1;

	//�f�o�b�K�̃p�����[�^���O�ǂݍ���
	KuroEngine::Debugger::ImportParameterLog();

	//�V�[�����X�g�i���[�U�[�ݒ�j=========
	std::map<std::string, KuroEngine::BaseScene*>sceneList =
	{
		{"title",new TitleScene()},
		{"Battle",new BattleScene()}
		//{"ProtoType",new GameScene()}
	};
	std::string awakeScene = "title";	//�J�n���̃V�[���L�[
	//================================

	//�G���W���ɃV�[�����X�g��n��
	engine.SetSceneList(sceneList, awakeScene);

	SoundConfig::Instance();

	bool winEnd = false;

	//���[�v
	while (1)
	{
		//���b�Z�[�W������H
		MSG msg{};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
			if (msg.message == WM_QUIT)
			{
				//�E�B���h�E������ꂽ
				winEnd = true;
				break;
			}
		}

		//�I�����b�Z�[�W������ / �V�[���}�l�[�W���̏I���@�Ń��[�v�𔲂���
		if (winEnd || engine.End())
		{
			break;
		}

		SoundConfig::Instance()->Update();
		engine.Update();
		engine.Draw();

		//�ÓI�N���X�������iDirty�t���O�n�j
		KuroEngine::Transform::DirtyReset();
		KuroEngine::Transform2D::DirtyReset();

		KuroEngine::DrawFunc2D::CountReset();
		KuroEngine::DrawFunc2D_Shadow::CountReset();
		KuroEngine::DrawFunc2D_FillTex::CountReset();
		KuroEngine::DrawFunc2D_Mask::CountReset();
		KuroEngine::DrawFunc2D_Color::CountReset();

		KuroEngine::DrawFunc3D::CountReset();

		KuroEngine::DrawFuncBillBoard::CountReset();
	}

	KuroEngine::Debugger::ExportParameterLog();

	return 0;
}