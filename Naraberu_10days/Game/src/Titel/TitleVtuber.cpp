#include "TitleVtuber.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Color.h"
#include "../RefreshRate.h"

//補間で使うデータ
std::array<KuroEngine::Vec2<float>, 4>smallPrismHokanPos = { {{0.0f, 0.0f},{10.0f, -20.0f},{5.0f, 5.0f},{-5.0f, 10.0f}} };
std::array<float, 4>smallPrismHokanAngle = { 0.0f,10.0f * (3.14f / 180.0f),-5.0f * (3.14f / 180.0f),-10.0f * (3.14f / 180.0f) };
std::array<KuroEngine::Vec2<float>, 4>bigPrismHokanPos_1 = { {{0.0f, 0.0f},{5.0f, -10.0f},{-5.0f, -15.0f},{5.0f, -5.0f}} };
std::array<KuroEngine::Vec2<float>, 3>bigPrismHokanPos_2 = { {{-600.0f,100.0f},{0.0f,200.0f},{-600.0f,300.0f}}};

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

TitleVtuber::TitleVtuber()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/title_scene/";

	characterTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "character.png");
	D3D12App::Instance()->GenerateTextureBuffer(smallPrismTex.data(), TexDir + "title_prism_particle.png", 3, { 3, 1 });
	D3D12App::Instance()->GenerateTextureBuffer(bigPrismTex.data(), TexDir + "title_block_particle.png", 3, { 3, 1 });

	characterShake.m_val = { 128.0f,128.0f };
	characterShake.m_interval = 300.0f;
}

void TitleVtuber::Initialize()
{
	for (int i = 0; i < 3; i++) {
		smallPrism[i].pos = smallPrismHokanPos[i];
		smallPrism[i].rota = 0.0f;
		smallPrism[i].number = 0;
		smallPrism[i].timer = 0.0f + 20.0f * (2 - i);
	}

	for (int i = 0; i < 3; i++) {
		bigPrism[i].pos = smallPrismHokanPos[i];
		bigPrism[i].rota = 0.0f;
		bigPrism[i].number = 2;
		bigPrism[i].timer = 0.0f + 20.0f * (2 - i);
		bigPrismInfo[i].useEase = true;
		bigPrismInfo[i].easeNum = 0;
		bigPrismInfo[i].back = false;

	}

	move = 0;
	//ステージセレクト選択で横にズレる
	isLeftMove = false;
	//タイトルに戻ると横にずれる
	isRightMove = false;
	//移動量
	move = 0;
	//タイマー
	moveTimer = 0;

	characterShake.Init();
}

void TitleVtuber::Update()
{
	SmallPrismAnimation();
	BigPrismAnimation();

	if (isLeftMove || isRightMove) {
		MoveStageSelect();
	} else {
		isMove = false;
	}

	characterShake.Update(1.0f, XMMatrixIdentity());
}

void TitleVtuber::Draw()
{
	using namespace KuroEngine;

	std::array<Vec2<float>,3> smallPrismPos;
	smallPrismPos[0] = { 546.0f,95.0f };
	smallPrismPos[1] = { 515.0f,145.0f };
	smallPrismPos[2] = { 470.0f,220.0f };
	
	std::array<Vec2<float>, 3> bigPrismPos;
	bigPrismPos[0] = { 1180.0f,95.0f };
	bigPrismPos[1] = { 1140.0f,220.0f };
	bigPrismPos[2] = { 1170.0f,360.0f };

	//小プリズム
	for (int i = 0; i < 3; i++) {
		DrawFunc2D::DrawRotaGraph2D({ smallPrismPos[i].x + smallPrism[i].pos.x + move,smallPrismPos[i].y + smallPrism[i].pos.y }, { 1.0f,1.0f }, smallPrism[i].rota, smallPrismTex[i]);
	}

	const std::array<Color, 3> prismColor = { {{553,254,137,130} ,{94,253,274,130},{255,93,204,130}} };

	//軌跡裏
	float size = 0;
	for (int i = 0; i < 3; i++) {
		for (auto& m : bigPrismInfo[i].trajectory) {
			size += 0.02f;
			if (m.back || !m.isTrajectoryDraw) { continue; }
			DrawFunc2D_Color::DrawRotaGraph2D({ bigPrismPos[i].x + m.pos.x + move,bigPrismPos[i].y + m.pos.y },
				{ 1.0f - size,1.0f - size }, 0.0f, bigPrismTex[i], prismColor[i]);
		}
	}

	//大プリズム裏
	for (int i = 0; i < 3; i++) {
		if (bigPrismInfo[i].back) { continue; }
		DrawFunc2D::DrawRotaGraph2D({ bigPrismPos[i].x + bigPrism[i].pos.x + move,bigPrismPos[i].y + bigPrism[i].pos.y }, { 1.0f,1.0f }, 0.0f, bigPrismTex[i]);
	}

	//キャラ
	Vec2<float>charaShakeAmount = { characterShake.GetOffset().x,characterShake.GetOffset().y };
	DrawFunc2D::DrawGraph(Vec2<float>(530.0f + move, -90.0f) + charaShakeAmount, characterTex);

	//軌跡表
	size = 0;
	for (int i = 0; i < 3; i++) {
		for (auto& m : bigPrismInfo[i].trajectory) {
			size += 0.02f;
			if (!m.back || !m.isTrajectoryDraw) { continue; }
			DrawFunc2D_Color::DrawRotaGraph2D({ bigPrismPos[i].x + m.pos.x + move,bigPrismPos[i].y + m.pos.y },
				{ 1.0f - size,1.0f - size }, 0.0f, bigPrismTex[i], prismColor[i]);
		}
	}

	//大プリズム表
	for (int i = 0; i < 3; i++) {
		if (!bigPrismInfo[i].back) { continue; }
		DrawFunc2D::DrawRotaGraph2D({ bigPrismPos[i].x + bigPrism[i].pos.x + move,bigPrismPos[i].y + bigPrism[i].pos.y }, { 1.0f,1.0f }, 0.0f, bigPrismTex[i]);
	}
}

void TitleVtuber::SmallPrismAnimation()
{
	const float maxTimer=50.0f * RefreshRate::RefreshRate_Mag;

	std::vector<KuroEngine::Vec2<float>> points = {
		smallPrismHokanPos[0], smallPrismHokanPos[0], smallPrismHokanPos[1], smallPrismHokanPos[2],
		smallPrismHokanPos[3], smallPrismHokanPos[0], smallPrismHokanPos[0], smallPrismHokanPos[1]
	};
	std::vector<float> rotas = {
		smallPrismHokanAngle[0], smallPrismHokanAngle[0], smallPrismHokanAngle[1], smallPrismHokanAngle[2],
		smallPrismHokanAngle[3], smallPrismHokanAngle[0], smallPrismHokanAngle[0], smallPrismHokanAngle[1]
	};

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
				i.timer = 0;
			}
		}
		i.pos = splinePosition(points, i.number, timeRate);
		i.rota= splinePosition(rotas, i.number, timeRate);
	}
}

void TitleVtuber::BigPrismAnimation()
{
	const float maxTimer = 60.0f * RefreshRate::RefreshRate_Mag;

	std::vector<KuroEngine::Vec2<float>> points_1 = {
	bigPrismHokanPos_1[0], bigPrismHokanPos_1[0], bigPrismHokanPos_1[1], bigPrismHokanPos_1[2],
	bigPrismHokanPos_1[3], bigPrismHokanPos_1[0], bigPrismHokanPos_1[0],
	bigPrismHokanPos_2[2], bigPrismHokanPos_2[1],bigPrismHokanPos_2[0], bigPrismHokanPos_1[0], bigPrismHokanPos_1[0], };

	for (int i=0;i<3;i++) {
		bigPrism[i].timer++;

		float timeRate = bigPrism[i].timer / maxTimer;

		if (timeRate >= 1.0f)
		{
			if (bigPrismInfo[i].useEase) {
				if (bigPrism[i].number < points_1.size() - 3 - 5) {
					bigPrism[i].number += 1;
					timeRate -= 1.0f;
					bigPrism[i].timer = 0.0f;
				} else {
					bigPrism[i].number = 0;
				}

				if (bigPrism[i].number == 0) {
					bigPrismInfo[i].easeNum++;
					if (bigPrismInfo[i].easeNum == 3) {
						bigPrismInfo[i].useEase = false;
					}
				}

				bigPrismInfo[i].trajectory[0].isTrajectoryDraw = false;
			} else {
				if (bigPrism[i].number < points_1.size() - 3) {
					bigPrism[i].number += 1;
					timeRate -= 1.0f;
					bigPrism[i].timer = 0.0f;
					bigPrismInfo[i].back = !bigPrismInfo[i].back;
				} else {
					bigPrism[i].number = 0;
				}
				if (bigPrism[i].number == 0) {
					bigPrismInfo[i].useEase = true;
					bigPrismInfo[i].back = false;
					bigPrismInfo[i].easeNum = 0;
				}

				if (bigPrism[i].number > 3) {
					bigPrismInfo[i].trajectory[0].isTrajectoryDraw = true;
				}
			}
		}

		//軌跡記録
		bigPrismInfo[i].trajectory[0].back = bigPrismInfo[i].back;
		bigPrismInfo[i].trajectory[0].pos = bigPrism[i].pos;

		for (int trajectoryNum = TrajectorySize-1; trajectoryNum > 0; trajectoryNum--) {
			bigPrismInfo[i].trajectory[trajectoryNum] = bigPrismInfo[i].trajectory[trajectoryNum - 1];
		}

		bigPrism[i].pos = splinePosition(points_1, bigPrism[i].number, timeRate);
	}
}

void TitleVtuber::MoveStageSelect()
{
	const float maxTimer = 40.0f * RefreshRate::RefreshRate_Mag;

	isMove = true;

	if (isLeftMove) {
		move = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			moveTimer, maxTimer, 0.0f, -1500.0f);
		if (moveTimer > maxTimer) {
			isLeftMove = false;
		}
	} else if(isRightMove){
		move = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
			moveTimer, maxTimer, -1500.0f, 0.0f);
		if (moveTimer > maxTimer) {
			isRightMove = false;
		}
	}

	moveTimer++;
}
