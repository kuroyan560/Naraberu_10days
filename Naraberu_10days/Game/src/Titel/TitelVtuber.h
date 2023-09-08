#pragma once
#include "KuroEngineDevice.h"
#include <array>

class TitelVtuber
{
	struct ObjectInfo {
		KuroEngine::Vec2<float> pos;
		float rota;
		float timer;
		int number;
	};

public:

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

};

