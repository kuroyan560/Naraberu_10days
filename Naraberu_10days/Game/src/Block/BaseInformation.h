#pragma once
#include <vector>
#include <array>
#include"Common/Vec.h"

static const float blockSize = 50.0f;
static const KuroEngine::Vec2<float> difference = { 391.0f,66.0f };//差

static const int shapeNum = 27;

static KuroEngine::Vec2<float> BlockCenter(std::vector<KuroEngine::Vec2<int>> _shape) {
	KuroEngine::Vec2<int> shapeMax = { 0,0 };
	KuroEngine::Vec2<int> shapeMin = { 0,0 };
	for (auto& i : _shape) {
		std::array<int, 4> moveHit = {
			int((i.x < shapeMin.x)),
			int((i.y < shapeMin.y)),
			int((i.x > shapeMax.x)),
			int((i.y > shapeMax.y))
		};

		shapeMin.x = moveHit[0] * i.x + (1 - moveHit[0]) * shapeMin.x;
		shapeMin.y = moveHit[1] * i.y + (1 - moveHit[1]) * shapeMin.y;
		shapeMax.x = moveHit[2] * i.x + (1 - moveHit[2]) * shapeMax.x;
		shapeMax.y = moveHit[3] * i.y + (1 - moveHit[3]) * shapeMax.y;
	}

	return { ((shapeMax.x + shapeMin.x)/2.0f) * 20.0f,((shapeMax.y + shapeMin.y) / 2.0f) * 20.0f };
}

//形状
static const std::vector<std::vector<KuroEngine::Vec2<int>>> shape = {
	{{0,0},{0,1},{1,0}},{{0,0},{0,-1},{-1,0}},{{0,0},{0,1},{-1,0}},{{0,0},{0,-1},{1,0}},//3つの角小(4)
	{{0,0},{0,1},{0,2},{1,0}},{{0,0},{0,-1},{0,-2},{1,0}},//3y直線端突右(2)
	{{0,0},{0,1},{0,2},{-1,0}},{{0,0},{0,-1},{0,-2},{-1,0}},//3y直線端突左(2)
	{{0,0},{1,0},{2,0},{0,1}},{{0,0},{-1,0},{-2,0},{0,1}},//3x直線端突上(2)
	{{0,0},{1,0},{2,0},{-1,0}},{{0,0},{-1,0},{-2,0},{0,-1}},//3x直線端突下(2)
	{{0,0},{0,1},{0,-1},{1,0}},{{0,0},{0,1},{0,-1},{-1,0}},{{0,0},{1,0},{-1,0},{0,1}},{{0,0},{1,0},{-1,0},{0,-1}},//Tスピンのやつ(4)
	{{0,0},{1,0},{2,0},{-1,0}},{{0,0},{0,1},{0,2},{0,-1}},//直線(2)
	{{0,0},{0,1},{0,2},{1,0},{2,0}},{{0,0},{0,-1},{0,-2},{-1,0},{-2,0}},
	{{0,0},{0,1},{0,2},{-1,0},{-2,0}},{{0,0},{0,-1},{0,-2},{1,0},{2,0}},//3つの角大(4)
	{{0,0},{0,1},{0,-1},{1,0},{-1,0}},//十字(1)
	{{0,0},{0,1},{0,-1},{1,1},{-1,-1}},{{0,0},{0,1},{0,-1},{1,-1},{-1,1}},
	{{0,0},{1,0},{-1,0},{1,1},{-1,-1}},{{0,0},{1,0},{-1,0},{1,-1},{-1,1}},//卍なり損ね(4)
	{{0,0},{-1,0},{1,0}},{{0,0},{0,-1},{0,1}},	//3直線
};

//形状による表示のズレ修正
static const std::vector<KuroEngine::Vec2<float>> shape_dist = {
	BlockCenter(shape[0]),BlockCenter(shape[1]),BlockCenter(shape[2]),BlockCenter(shape[3]),//3つの角小(4)
	BlockCenter(shape[4]),BlockCenter(shape[5]),//3y直線端突右(2)
	BlockCenter(shape[6]),BlockCenter(shape[7]),//3y直線端突左(2)
	BlockCenter(shape[8]),BlockCenter(shape[9]),//3x直線端突上(2)
	BlockCenter(shape[10]),BlockCenter(shape[11]),//3x直線端突下(2)
	BlockCenter(shape[12]),BlockCenter(shape[13]),BlockCenter(shape[14]),BlockCenter(shape[15]),//Tスピンのやつ(4)
	BlockCenter(shape[16]),BlockCenter(shape[17]),//直線(2)
	BlockCenter(shape[18]),BlockCenter(shape[19]),BlockCenter(shape[20]),BlockCenter(shape[21]),//3つの角大(4)
	BlockCenter(shape[22]),//十字(1)
	BlockCenter(shape[23]),BlockCenter(shape[24]),BlockCenter(shape[25]),BlockCenter(shape[26])//卍なり損ね(4)
};

//色
enum class BlockColor {
	red = 0,
	blue,
	yellow,
	yuka,
	obstacle,
	eizoku_obstacle,
	gold,
	size,
};

//属性
enum class BlockAttribute {
	attack1,
	attack2,
	recovery,
	size
};