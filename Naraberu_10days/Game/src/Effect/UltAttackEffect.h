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
	//�p�[�e�B�N�����o
	bool isParticle;
	//�g��
	bool isWaves;
	//�傫��
	float dist;
	//�^�C�}�[
	float timer;
	//�p�[�e�B�N����
	static const int particleNum = 250;
	//�p�[�e�B�N�����
	std::array<Particle, particleNum> particle;
};

