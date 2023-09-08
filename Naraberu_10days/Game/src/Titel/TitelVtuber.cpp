#include "TitelVtuber.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

//補間で使うデータ
std::array<KuroEngine::Vec2<float>, 4>hokanPos = { {{0.0f, 0.0f},{10.0f, -20.0f},{5.0f, 5.0f},{-5.0f, 10.0f}} };
std::array<float, 4>hokanAngle = { 0.0f,10.0f * (3.14f / 180.0f),-5.0f * (3.14f / 180.0f),-10.0f * (3.14f / 180.0f) };

//制御点の集合(vectorコンテナ)、保管する区間の添字、時間経過
KuroEngine::Vec2<float> splinePosition(const std::vector<KuroEngine::Vec2<float>>& points, size_t startIndex, float t) {
	using namespace KuroEngine;
	//補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n) { return points[n]; }
	if (startIndex < 1) { return points[1]; }

	//p0~p3の制御点を取得する※p1~p2を補間する
	Vec2<float> p0 = points[startIndex - 1];
	Vec2<float> p1 = points[startIndex];
	Vec2<float> p2 = points[startIndex + 1];
	Vec2<float> p3 = points[startIndex + 2];

	//Catmull-Romの式による補間
	Vec2<float> position = (p1 * 2 + (-p0 + p2) * t
		+ (p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t
		+ (-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t) * 0.5;

	return position;
}

//制御点の集合(vectorコンテナ)、保管する区間の添字、時間経過
float splinePosition(const std::vector<float>& points, size_t startIndex, float t) {
	using namespace KuroEngine;
	//補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n) { return points[n]; }
	if (startIndex < 1) { return points[1]; }

	//p0~p3の制御点を取得する※p1~p2を補間する
	float p0 = points[startIndex - 1];
	float p1 = points[startIndex];
	float p2 = points[startIndex + 1];
	float p3 = points[startIndex + 2];

	//Catmull-Romの式による補間
	float position = (p1 * 2.0f + (-p0 + p2) * t
		+ (p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t * t
		+ (-p0 + p1 * 3.0f - p2 * 3.0f + p3) * t * t * t) * 0.5f;

	return position;
}

void TitelVtuber::Initialize()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/title_scene/";

	characterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "character.png");
	D3D12App::Instance()->GenerateTextureBuffer(smallPrismTex.data(), TexDir + "title_prism_particle.png", 3, { 3, 1 });
	D3D12App::Instance()->GenerateTextureBuffer(bigPrismTex.data(), TexDir + "title_block_particle.png", 3, { 3, 1 });

	for (int i = 0; i < 3; i++) {
		smallPrism[i].pos = hokanPos[i];
		smallPrism[i].rota = 0.0f;
		smallPrism[i].number = 0;
		smallPrism[i].timer = 0.0f + 20.0f * (2 - i);
	}
}

void TitelVtuber::Update()
{
	SmallPrismAnimation();
}

void TitelVtuber::Draw()
{
	using namespace KuroEngine;
	DrawFunc2D::DrawGraph({ 530.0f,-150.0f }, characterTex);

	std::array<Vec2<float>,3> pos;
	pos[0] = { 546.0f,95.0f };
	pos[1] = { 515.0f,145.0f };
	pos[2] = { 470.0f,220.0f };

	for (int i = 0; i < 3; i++) {
		//DrawFunc2D::DrawRotaGraph2D({pos[i]},{1.0f,1.0f}, smallPrism[i].rota, smallPrismTex[i]);
		DrawFunc2D::DrawRotaGraph2D({ pos[i].x + smallPrism[i].pos.x,pos[i].y + smallPrism[i].pos.y }, { 1.0f,1.0f }, smallPrism[i].rota, smallPrismTex[i]);
	}

}

void TitelVtuber::SmallPrismAnimation()
{
	const float maxTimer=50.0f;

	std::vector<KuroEngine::Vec2<float>> points = {
	hokanPos[0], hokanPos[0], hokanPos[1], hokanPos[2], hokanPos[3], hokanPos[0], hokanPos[0],hokanPos[1] };
	std::vector<float> rotas = {
	hokanAngle[0], hokanAngle[0], hokanAngle[1], hokanAngle[2], hokanAngle[3], hokanAngle[0], hokanAngle[0],hokanAngle[1] };

	for (auto& i : smallPrism) {
		i.timer++;

		float timeRate = i.timer / maxTimer;

		if (timeRate >= 1.0f)
		{
			if (i.number < points.size() - 3) {
				i.number += 1;
				timeRate -= 1.0f;
				i.timer = 0.0f;
			} else {
				i.number = 0;
			}
		}
		i.pos = splinePosition(points, i.number, timeRate);
		i.rota= splinePosition(rotas, i.number, timeRate);
	}

}

void TitelVtuber::BigPrismAnimation()
{
}
