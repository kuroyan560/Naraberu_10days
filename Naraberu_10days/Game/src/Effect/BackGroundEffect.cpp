#include "../Effect/BackGroundEffect.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"

void BackGroundEffect::Initialize()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/back_ground/";

	for (int i = 0; i < 3; i++) {
		triangleTex[i] = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "triangle" + std::to_string(i + 1) + ".png");
	}

	const KuroEngine::Vec2<float> max = KuroEngine::WinApp::Instance()->GetExpandWinSize();
	for (auto& i : info) {
		i.isAlive = 0;
		i.pos = { KuroEngine::GetRand(-20.0f, max.x + 20.0f),KuroEngine::GetRand(-20.0f, max.y + 20.0f) };
		i.scale = { KuroEngine::GetRand(0.2f, 0.6f),KuroEngine::GetRand(0.2f, 0.6f) };
		i.angle = KuroEngine::GetRand(0.0f, 360.0f);
		i.angle = i.angle * (3.14f / 180.0f);
		i.alpha = 0.0f;
		i.texNum = KuroEngine::GetRand(3);
	}
}

void BackGroundEffect::Update()
{
	//Œõ’Ç‰Á
	int count = 0;
	if (timer % 30 == 0) {
		while (count < 1) {
			int num = KuroEngine::GetRand(maxNum - 1);

			if (info[num].isAlive != 0) { continue; }
			info[num].isAlive = 1;
			count++;
		}
		timer = 0;
	}
	timer++;

	const float upDown = 0.001f;
	const float max = 0.3f;
	for (auto& i : info) {
		if (i.isAlive == 1) {
			i.alpha += upDown;
			if (i.alpha > max) {
				i.isAlive = 2;
			}
		} else if (i.isAlive == 2) {
			i.alpha -= upDown;
			if (i.alpha <= 0) {
				i.isAlive = 0;
			}
		}
	}
}

void BackGroundEffect::Draw()
{
	Update();
	for (auto& i : info) {
		if (!i.isAlive) { continue; }
		KuroEngine::DrawFunc2D_Color::DrawRotaGraph2D(i.pos,i.scale,i.angle, triangleTex[i.texNum],{1.0f,1.0f,1.0f,i.alpha});
	}
}
