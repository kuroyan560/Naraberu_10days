#pragma once
#include "KuroEngineDevice.h"
#include <array>
#include"ForUser/HandShake.h"

class TitleVtuber
{
	static const int TrajectorySize=10;
	struct ObjectInfo {
		KuroEngine::Vec2<float> pos;
		float rota;
		float timer;
		int number;
	};

	struct Trajectory {
		//軌跡描画
		bool isTrajectoryDraw;
		//座標
		KuroEngine::Vec2<float> pos;
		//裏表
		bool back;
	};

	struct BigPrismInfo {
		//補間回数
		int easeNum;
		//どちらの補間を使用するか
		bool useEase;
		//裏表
		bool back;
		//軌跡
		std::array<Trajectory, TrajectorySize> trajectory;
	};

public:
	TitleVtuber();

	void Initialize();

	void Update();

	void Draw();

	//プリズム小アニメーション
	void SmallPrismAnimation();

	//プリズム大アニメーション
	void BigPrismAnimation();

	void MoveStageSelect();

	//ステージセレクト選択で横にズレる
	void SetMoveStageSelect(bool _mouzureteru = false) {
		if (_mouzureteru) {
			move = -1500.0f;
			LR = false;
			isRightMove = false;
		} else {
			isLeftMove = true;
			moveTimer = 0;
		}
	}

	//タイトルに戻る時
	void SetMoveTitle(bool _mouzureteru = false) {
		if (_mouzureteru) {
			move = 0.0f;
			LR = true;
		} else {
			isRightMove = true;
			moveTimer = 0;
		}
	}

	float GetMove() { return move; }

private:

	//キャラ
	std::shared_ptr<KuroEngine::TextureBuffer> characterTex;
	//小さいプリズム
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,3> smallPrismTex;
	//大きいプリズム
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 3> bigPrismTex;

	//小さいプリズム
	std::array<ObjectInfo,3> smallPrism;
	//大きいプリズム
	std::array<ObjectInfo, 3> bigPrism;
	//どちらの補間を使用するか
	std::array<BigPrismInfo,3> bigPrismInfo;

	//キャラの揺れ
	KuroEngine::HandShake characterShake;

	//ステージセレクト選択で横にズレる
	bool isLeftMove;
	//タイトルに戻ると横にずれる
	bool isRightMove;
	//移動量
	float move;
	//タイマー
	float moveTimer;
	//どちらにいるかR true L false
	bool LR;
};

