#include "Block.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/UsersInput.h"
#include "BaseInformation.h"
#include "StageManager.h"

Block::Block(bool _isMove)
{
	//‰æ‘œ“Ç‚Ýž‚Ý
	std::string TexDir = "resource/user/tex/block/";
	blockTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_pink.png");
	blockTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_blue.png");
	blockTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_yellow.png");

	isMove = _isMove;
}

void Block::Update()
{
}

void Block::Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const int _texNum, const BlockColor _color)
{
	for (auto& i : _shape) {
		KuroEngine::Vec2<float> inpos = { (pos.x + i.x) * blockSize + difference.x , (pos.y + i.y) * blockSize + difference.y };
		BlockOneDraw(inpos, _color);
	}
}

void Block::Draw(const std::vector<KuroEngine::Vec2<int>> _shape,
	const int _texNum, const BlockColor _color, const KuroEngine::Vec2<float>& _pos)
{
	for (auto& i : _shape) {
		BlockOneDraw(_pos, _color);
	}
}

void Block::Reset()
{
}

void Block::Move()
{
	
	KuroEngine::Vec2<int> max = StageManager::GetMapMax();
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();
	if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::DPAD_LEFT)) {
		if (pos.x <= -shapeMin.x) { return; }
		pos.x -= 1;
	} else if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::DPAD_RIGHT)) {
		if (pos.x >= max.x - 1 - shapeMax.x) { return; }
		pos.x += 1;
	} else if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::DPAD_UP)) {
		if (pos.y <= -shapeMin.y) { return; }
		pos.y -= 1;
	} else if (input->ControllerOnTrigger(0, KuroEngine::XBOX_BUTTON::DPAD_DOWN)) {
		if (pos.y >= max.y - 1 - shapeMax.y) { return; }
		pos.y += 1;
	}
	input = nullptr;

}

void Block::ChangeBlock(const KuroEngine::Vec2<int> _mapchipNum, const std::vector<KuroEngine::Vec2<int>> _shape)
{
	pos = _mapchipNum;
	shapeMax = { 0,0 };
	shapeMin = { 0,0 };
	for (auto& i : _shape) {
		shapeMin.x = int((i.x < shapeMin.x)) * i.x + shapeMin.x;
		shapeMin.y = int((i.y < shapeMin.y)) * i.y + shapeMin.y;
		shapeMax.x = int((i.x > shapeMax.x)) * i.x + shapeMax.x;
		shapeMax.y = int((i.y > shapeMax.y)) * i.y + shapeMax.y;
	}
}

void Block::BlockOneDraw(const KuroEngine::Vec2<float> pos, BlockColor _color)
{
	using namespace KuroEngine;

	Vec2<float> pos1 = pos;
	pos1.x += blockSize;
	pos1.y += blockSize;

	if (_color == BlockColor::red) {
		DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::red)]);
	} else if (_color == BlockColor::blue) {
		DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::blue)]);
	} else if (_color == BlockColor::yellow) {
		DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::yellow)]);
	}
}
