#pragma once
#include "KuroEngineDevice.h"
#include <array>
#include"ForUser/HandShake.h"

class TitleVtuber
{
	struct ObjectInfo {
		KuroEngine::Vec2<float> pos;
		float rota;
		float timer;
		int number;
	};

	struct BigPrismInfo {
		//補間回数
		int easeNum;
		//どちらの補間を使用するか
		bool useEase;
		//裏表
		bool back;
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

};

