#pragma once
#include <vector>
#include <array>
#include"Common/Vec.h"

static const float blockSize = 50.0f;
static const KuroEngine::Vec2<float> difference = { 391.0f,66.0f };//��

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

//�`��
static const std::vector<std::vector<KuroEngine::Vec2<int>>> shape = {
	{{0,0},{0,1},{1,0}},{{0,0},{0,-1},{-1,0}},{{0,0},{0,1},{-1,0}},{{0,0},{0,-1},{1,0}},//3�̊p��(4)
	{{0,0},{0,1},{0,2},{1,0}},{{0,0},{0,-1},{0,-2},{1,0}},//3y�����[�ˉE(2)
	{{0,0},{0,1},{0,2},{-1,0}},{{0,0},{0,-1},{0,-2},{-1,0}},//3y�����[�ˍ�(2)
	{{0,0},{1,0},{2,0},{0,1}},{{0,0},{-1,0},{-2,0},{0,1}},//3x�����[�ˏ�(2)
	{{0,0},{1,0},{2,0},{-1,0}},{{0,0},{-1,0},{-2,0},{0,-1}},//3x�����[�ˉ�(2)
	{{0,0},{0,1},{0,-1},{1,0}},{{0,0},{0,1},{0,-1},{-1,0}},{{0,0},{1,0},{-1,0},{0,1}},{{0,0},{1,0},{-1,0},{0,-1}},//T�X�s���̂��(4)
	{{0,0},{1,0},{2,0},{-1,0}},{{0,0},{0,1},{0,2},{0,-1}},//����(2)
	{{0,0},{0,1},{0,2},{1,0},{2,0}},{{0,0},{0,-1},{0,-2},{-1,0},{-2,0}},
	{{0,0},{0,1},{0,2},{-1,0},{-2,0}},{{0,0},{0,-1},{0,-2},{1,0},{2,0}},//3�̊p��(4)
	{{0,0},{0,1},{0,-1},{1,0},{-1,0}},//�\��(1)
	{{0,0},{0,1},{0,-1},{1,1},{-1,-1}},{{0,0},{0,1},{0,-1},{1,-1},{-1,1}},
	{{0,0},{1,0},{-1,0},{1,1},{-1,-1}},{{0,0},{1,0},{-1,0},{1,-1},{-1,1}},//�Ȃ葹��(4)
	{{0,0},{-1,0},{1,0}},{{0,0},{0,-1},{0,1}},	//3����
};

//�`��ɂ��\���̃Y���C��
static const std::vector<KuroEngine::Vec2<float>> shape_dist = {
	BlockCenter(shape[0]),BlockCenter(shape[1]),BlockCenter(shape[2]),BlockCenter(shape[3]),//3�̊p��(4)
	BlockCenter(shape[4]),BlockCenter(shape[5]),//3y�����[�ˉE(2)
	BlockCenter(shape[6]),BlockCenter(shape[7]),//3y�����[�ˍ�(2)
	BlockCenter(shape[8]),BlockCenter(shape[9]),//3x�����[�ˏ�(2)
	BlockCenter(shape[10]),BlockCenter(shape[11]),//3x�����[�ˉ�(2)
	BlockCenter(shape[12]),BlockCenter(shape[13]),BlockCenter(shape[14]),BlockCenter(shape[15]),//T�X�s���̂��(4)
	BlockCenter(shape[16]),BlockCenter(shape[17]),//����(2)
	BlockCenter(shape[18]),BlockCenter(shape[19]),BlockCenter(shape[20]),BlockCenter(shape[21]),//3�̊p��(4)
	BlockCenter(shape[22]),//�\��(1)
	BlockCenter(shape[23]),BlockCenter(shape[24]),BlockCenter(shape[25]),BlockCenter(shape[26])//�Ȃ葹��(4)
};

//�F
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

//����
enum class BlockAttribute {
	attack1,
	attack2,
	recovery,
	size
};