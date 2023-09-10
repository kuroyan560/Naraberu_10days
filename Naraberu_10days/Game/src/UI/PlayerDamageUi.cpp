#include "PlayerDamageUi.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../RefreshRate.h"

void PlayerDamageUi::Initialize()
{
	std::string TexDir = "resource/user/tex/battle_scene/";
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(numberTex.data(), TexDir + "player_damage.png", 11, { 11, 1 });

	pos = { 40.0f ,0.0f };
}

void PlayerDamageUi::Update()
{
	if (!isDamage) { return; }

	const float maxTimer = 20.0f * RefreshRate::RefreshRate_Mag;

	timer++;

	if (timer > maxTimer + 10.0f) {
		isDamage = false;
		return;
	}

	if (timer > maxTimer) { return; }

	pos.y = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Quart,
		timer, maxTimer, 400.0f, 370.0f);

	alpha = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Quart,
		timer, maxTimer, 0.0f, 1.0f);
}

void PlayerDamageUi::Draw()
{
	if (!isDamage) { return; }

	KuroEngine::DrawFunc2D::DrawNumber2D(damageNum, pos, numberTex.data(), { 1.0f,1.0f }, alpha, 0.0f,
		KuroEngine::HORIZONTAL_ALIGN::LEFT, KuroEngine::VERTICAL_ALIGN::TOP, -1, 10);
}
