#include "../Effect/UltAttackEffect.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../RefreshRate.h"

void UltAttackEffect::Initialize()
{
	std::string TexDir = "resource/user/tex/battle_scene/";
	particleTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "particle.png");

	const KuroEngine::Vec2<float> pos = KuroEngine::WinApp::Instance()->GetExpandWinSize();
	for (auto& i : particle) {
		i.pos = { float(rand() % int(pos.x)),float(rand() % int(pos.y)) };
		i.isAlive = 0;
		i.alpha = 0.0f;
		i.timer = 0.0f;
	}

	isParticle = false;
	isWaves = false;
}

void UltAttackEffect::Update()
{
	//particle
	if (isParticle) {
		//�p�[�e�B�N���ǉ�
		if (timer < 10) {
			int count = 0;
			while (count < 20) {
				int i = rand() % particleNum;
				//���ɕ`�悵�Ă���Ȃ�X�L�b�v
				if (particle[i].isAlive > 0) { continue; }
				particle[i].isAlive = 1;
				particle[i].alpha = true;
				count++;
			}
		}
		const float maxTimer = 5.0f * RefreshRate::RefreshRate_Mag;
		for (auto& i : particle) {
			if (i.isAlive == 1) {
				dist = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Sine,
					timer, maxTimer, 0.0f, 0.5f);
				i.timer++;
				if (i.timer > maxTimer) {
					i.timer = 0;
					i.isAlive = 2;
				}
			}else if (i.isAlive == 2) {
				dist = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Sine,
					timer, maxTimer, 0.5f, 0.0f);
				i.timer++;
				if (i.timer > maxTimer) {
					i.timer = 0;
					i.isAlive = 0;
				}
			}
		}

		timer++;

		if (timer > 30) {
			timer = 0;
			isParticle = false;
			isWaves = true;
		}
	}

	//�g��
	if (isWaves) {
		const float maxTimer = 30.0f * RefreshRate::RefreshRate_Mag;
		dist = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Exp,
			timer, maxTimer, 0.0f, 1500.0f);
		timer++;
		if (timer > maxTimer) {
			isWaves = false;
		}
	}
}

void UltAttackEffect::Draw()
{
	if (isParticle) {
		for (auto& i : particle) {
			if (i.isAlive == 0) { continue; }
			KuroEngine::DrawFunc2D::DrawGraph(i.pos, particleTex,i.alpha,0.2f, KuroEngine::AlphaBlendMode::AlphaBlendMode_Add);
		}
	}

	if (!isWaves) { return; }
	KuroEngine::Color color = { 19,203,247,180 };

	for (int i = 0; i < 10; i++) {
		KuroEngine::DrawFunc2D::DrawCircle2D({ 306.0f,70.0f }, dist - i * 5, color, false, 80 - i * 4, KuroEngine::AlphaBlendMode::AlphaBlendMode_Add);
	}
}