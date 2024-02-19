#pragma once
#include "KuroEngineDevice.h"

#include <array>

class TitleUi
{
private:

	enum class POS {
		up,
		center,
		down,
		non,
	};

	struct StageTexInfo {
		KuroEngine::Vec2<float> pos;
		float scale;
		POS nowPos;
		POS nextPos;
	};

public:
	TitleUi();

	void Initialize();

	void Update();

	void Draw();

	/// <summary>
	/// ステージセレクトに移動するときの処理
	/// </summary>
	void StageSelectInMove();

	/// <summary>
	/// タイトルに移動するときの処理
	/// </summary>
	void StageSelectOutMove();

	/// <summary>
	/// ステージセレクト移動
	/// </summary>
	/// <param name="_number"></param>
	void StageSelectMove(int _number);
	
	/// <summary>
	/// ステージセレクト移動入れ
	/// </summary>
	/// <param name="_upDown">false->低い番号に行く(down) true->高い番号に行く(up)</param>
	/// <returns>選択が変化したか</returns>
	bool SetStageSelectMove(int _upDown);

	void SetIsStageSelectInMove(bool _isStageSelectInMove = true) { isStageSelectInMove = _isStageSelectInMove; }
	void SetIsStageSelectOutMove(bool _isStageSelectOutMove = true) { isStageSelectOutMove = _isStageSelectOutMove; }
	bool GetIsStageMove() { return isStageMove; }
	int GetStageNum() { return nowStageNum + 1; }
	bool GetIsStageSelectInMove() { return isStageSelectInMove; }
	bool GetIsStageSelectOutMove() { return isStageSelectOutMove; }

	// 背景
	std::shared_ptr<KuroEngine::TextureBuffer> backTex;
	//タイトル
	std::shared_ptr<KuroEngine::TextureBuffer> titleTex;
	//選択肢
	std::shared_ptr<KuroEngine::TextureBuffer> stageTex;
	//ステージ選択バー
	std::shared_ptr<KuroEngine::TextureBuffer> stageSelectBarTex;
	//数字
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11> numTex;
	//数字
	std::shared_ptr<KuroEngine::TextureBuffer> tutorialTex;
	//クリア
	std::shared_ptr<KuroEngine::TextureBuffer> challengeTex;
	//クリア
	std::shared_ptr<KuroEngine::TextureBuffer> clearTex;

	//タイトルからステージセレクトへ移動する
	bool isStageSelectInMove;
	float stageSelectInMoveTimer;
	//ステージセレクトからタイトルへ移動する
	bool isStageSelectOutMove;
	float stageSelectOutMoveTimer;
	//ステージセレクトの移動を行う
	bool isStageMove;
	//セレクト移動
	float selectTimer;
	//現在のステージ番号
	int nowStageNum;
	//ステージ変化+-判定
	bool stageUpDown;
	//ステージ数
	static const int stage = 8;
	//セレクト情報
	std::array<StageTexInfo, stage> stageTexInfo;
};

