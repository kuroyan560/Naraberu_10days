#pragma once
#include "KuroEngineDevice.h"
#include "../TitleUi.h"
#include "../Fade/Fade.h"
#include "../Titel/TitelVtuber.h"

class TitleScene : public KuroEngine::BaseScene
{	
	//UI
	std::shared_ptr<TitleUi> title;

	//キャラクターアニメーション
	std::shared_ptr<TitelVtuber> character;

	//タイトル
	std::shared_ptr<KuroEngine::TextureBuffer> titleTex;
	//start文字
	std::shared_ptr<KuroEngine::TextureBuffer> startTex;
	//quit文字
	std::shared_ptr<KuroEngine::TextureBuffer> quitTex;
	//title選択バー
	std::shared_ptr<KuroEngine::TextureBuffer> titleBarTex;
	//ステージ選択バー
	std::shared_ptr<KuroEngine::TextureBuffer> stageSelectBarTex;
	//透かし（後で消す）
	std::shared_ptr<KuroEngine::TextureBuffer> m_SukasiTex;

	//ステージ番号
	int stageNum;

	// フェード
	Fade m_Fade;

	//タイトルシーン用
	enum class Select {
		title,
		stageSelect,
		size,
	};

	// 選択肢
	Select selectNum;
	// 選択を行っている
	bool onSelect;

	/// <summary>
	/// タイトル
	/// </summary>
	void Title();

	/// <summary>
	/// ステージセレクト
	/// </summary>
	void StageSelect();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	TitleScene();

};

