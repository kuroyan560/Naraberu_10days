#pragma once
#include <vector>
#include"Common/Vec.h"

static const float blockSize = 50.0f;
static const KuroEngine::Vec2<float> difference = { 393.0f,114.0f };//·

//F
enum class BlockColor {
	red,
	blue,
	yellow,
	size,
};

//‘®«
enum class BlockAttribute {
	attack1,
	attack2,
	recovery,
	size
};