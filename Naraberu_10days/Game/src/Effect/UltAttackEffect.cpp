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
		i.a_pos = { float(rand() % int(pos.x)) - float(pos.x / 2.0f),float(rand() % int(pos.y)) - float(pos.y / 2.0f) };
		i.pos = i.a_pos;
		i.isAlive = 0;
		i.alpha = 1.0f;
		i.timer = 0.0f;
	}

	isParticle = false;
	isWaves = false;
}

void UltAttackEffect::Update()
{
	//particle
	if (isParticle) {
		//パーティクル追加
		if (timer < 10) {
			int count = 0;
			while (count < 20) {
				int i = rand() % particleNum;
				//既に描画しているならスキップ
				if (particle[i].isAlive) { continue; }
				particle[i].isAlive = true;
				count++;
			}
		}
		const float maxTimer = 10.0f * RefreshRate::RefreshRate_Mag;
		for (auto& i : particle) {
			if (i.isAlive == 1) {
				i.alpha = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Sine,
					timer, maxTimer, 1.0f, 0.0f);
				i.pos = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Sine,
					timer, maxTimer, i.a_pos, { 306.0f,70.0f });

				i.timer++;
				if (i.timer > maxTimer) {
					i.timer = 0;
					i.isAlive = false;
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

	//波動
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