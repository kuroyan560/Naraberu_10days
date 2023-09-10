#pragma once
#include "KuroEngineDevice.h"
#include <array>

class UltAttackEffect
{
private:

	struct Particle {
		KuroEngine::Vec2<float> pos;
		int isAlive;
		float alpha;
		float timer;
	};

public:
	void Initialize();

	void Update();

	void Draw();

	void SetUltAttack(){
		isParticle = true;
		timer = 0;
	}

private:
	
	std::shared_ptr<KuroEngine::TextureBuffer> particleTex;
	//パーティクル演出
	bool isParticle;
	//波動
	bool isWaves;
	//大きさ
	float dist;
	//タイマー
	float timer;
	//パーティクル個数
	static const int particleNum = 250;
	//パーティクル情報
	std::array<Particle, particleNum> particle;
};

