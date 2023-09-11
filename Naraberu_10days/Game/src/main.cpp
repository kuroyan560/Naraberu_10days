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
//Windowsアプリでのエントリーポイント（main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	//エンジン設定（ユーザー設定）========
	KuroEngine::EngineOption engineOption;
	engineOption.m_windowName = "マジカプリズム";
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

	//エンジン起動（失敗したら終了）
	KuroEngine::KuroEngineDevice engine;
	if (engine.Initialize(engineOption))return 1;

	//デバッガのパラメータログ読み込み
	KuroEngine::Debugger::ImportParameterLog();

	//シーンリスト（ユーザー設定）=========
	std::map<std::string, KuroEngine::BaseScene*>sceneList =
	{
		{"title",new TitleScene()},
		{"Battle",new BattleScene()}
		//{"ProtoType",new GameScene()}
	};
	std::string awakeScene = "title";	//開始時のシーンキー
	//================================

	//エンジンにシーンリストを渡す
	engine.SetSceneList(sceneList, awakeScene);

	SoundConfig::Instance();

	bool winEnd = false;

	//ループ
	while (1)
	{
		//メッセージがある？
		MSG msg{};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	//キー入力メッセージの処理
			DispatchMessage(&msg);	//プロシージャにメッセージを送る
			if (msg.message == WM_QUIT)
			{
				//ウィンドウが閉じられた
				winEnd = true;
				break;
			}
		}

		//終了メッセージが来た / シーンマネージャの終了　でループを抜ける
		if (winEnd || engine.End())
		{
			break;
		}

		SoundConfig::Instance()->Update();
		engine.Update();
		engine.Draw();

		//静的クラス初期化（Dirtyフラグ系）
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