#pragma once
#include "KuroEngineDevice.h"
#include "../TitleUi.h"

class TitleScene : public KuroEngine::BaseScene
{
	enum class Select {
		title,
		stageSelect,
		size,
	};
	
	//UI
	std::shared_ptr<TitleUi> title;

	//ステージ選択バー
	std::shared_ptr<KuroEngine::TextureBuffer> stageSelectBarTex;

	//選択肢
	Select selectNum;

	//選択を行っている
	bool onSelect;

	//ステージ番号
	int stageNum;

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

