#include "Block.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/UsersInput.h"
#include "BaseInformation.h"
#include "../Panel/PanelManager.h"
#include"../OperationConfig.h"

Block::Block(bool _isMove)
{
	//âÊëúì«Ç›çûÇ›
	std::string TexDir = "resource/user/tex/block/";
	blockTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_pink.png");
	blockTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_blue.png");
	blockTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_yellow.png");

	lineTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "blind_pink.png");
	lineTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "blind_blue.png");
	lineTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "blind_yellow.png");

	std::string acDir = "action_icon/";
	actionTex[int(BlockAttribute::attack1)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + acDir + "weak_attack_icon.png");
	actionTex[int(BlockAttribute::attack2)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + acDir + "strong_attack_icon.png");
	actionTex[int(BlockAttribute::recovery)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + acDir + "heal_icon.png");

	isMove = _isMove;
}

void Block::Update()
{
}

void Block::Draw(const std::vector<KuroEngine::Vec2<int>> _shape, BlockAttribute _attribute, const BlockColor _color)
{
	for (auto& i : _shape) {
		KuroEngine::Vec2<float> inpos = { (pos.x + i.x) * blockSize + difference.x , (pos.y + i.y) * blockSize + difference.y };
		BlockOneDraw(inpos, _color);
	}

	ActionDraw({ (pos.x + shapeMax.x) * blockSize + difference.x, (pos.y + shapeMin.y) * blockSize + difference.y }, _attribute);
}

void Block::Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const KuroEngine::Vec2<float> shape_dist,
	BlockAttribute _attribute, const BlockColor _color, const KuroEngine::Vec2<float>& _pos)
{
	KuroEngine::Vec2<float> pos = { _pos.x - shape_dist.x,_pos.y - shape_dist.y };
	for (auto& i : _shape) {
		BlockOneDraw(i, pos, _color);
	}

	ActionDraw({ shapeMax.x * blockSize + _pos.x, shapeMin.y * blockSize + _pos.y }, _attribute);
}

void Block::Reset()
{
}

void Block::Move()
{
	KuroEngine::Vec2<int> max = PanelManager::GetMapMax();
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();
	if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_LEFT)) {
		if (pos.x <= -shapeMin.x) { return; }
		pos.x -= 1;
	} else if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_RIGHT)) {
		if (pos.x >= max.x - 1 - shapeMax.x) { return; }
		pos.x += 1;
	} else if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_UP)) {
		if (pos.y <= -shapeMin.y) { return; }
		pos.y -= 1;
	} else if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_DOWN)) {
		if (pos.y >= max.y - 1 - shapeMax.y) { return; }
		pos.y += 1;
	}
	input = nullptr;
}

void Block::ChangeBlock(const KuroEngine::Vec2<int> _mapchipNum, const std::vector<KuroEngine::Vec2<int>> _shape)
{
	//ç≈è¨ç≈ëÂÇïœçX
	shapeMax = { 0,0 };
	shapeMin = { 0,0 };
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

	if (_mapchipNum.x != -1) {
		pos = _mapchipNum;
	} else {
		KuroEngine::Vec2<int> max = PanelManager::GetMapMax();

		//ìÀÇ´èoÇƒÇ¢ÇΩèÍçáñﬂÇ∑
		pos.x += int(pos.x + shapeMin.x < 0) * abs(pos.x + shapeMin.x);
		pos.x += int(shapeMax.x + pos.x >= max.x) * (shapeMax.x + pos.x - max.x - 1);
		pos.y += int(pos.y + shapeMin.y < 0) * abs(pos.y + shapeMin.y);
		pos.y += int(shapeMax.y + pos.y >= max.y) * (shapeMax.y + pos.y - max.y - 1);
	}
}

void Block::BlockOneDraw(const KuroEngine::Vec2<float> _pos, BlockColor _color)
{
	using namespace KuroEngine;

	Vec2<float> pos1 = _pos;
	pos1.x += blockSize;
	pos1.y += blockSize;

	if (_color == BlockColor::red) {
		DrawFunc2D::DrawExtendGraph2D(_pos, pos1, lineTex[int(BlockColor::red)]);
	} else if (_color == BlockColor::blue) {
		DrawFunc2D::DrawExtendGraph2D(_pos, pos1, lineTex[int(BlockColor::blue)]);
	} else if (_color == BlockColor::yellow) {
		DrawFunc2D::DrawExtendGraph2D(_pos, pos1, lineTex[int(BlockColor::yellow)]);
	}

}

void Block::BlockOneDraw(const KuroEngine::Vec2<int> _shape, const KuroEngine::Vec2<float> pos, const BlockColor _color)
{
	using namespace KuroEngine;
	const float blockSizeUnder = 20.0f;

	Vec2<float> pos1 = pos;
	pos1.x += _shape.x * blockSizeUnder;
	pos1.y += _shape.y * blockSizeUnder;

	Vec2<float> pos2 = pos;
	pos2.x += blockSizeUnder + _shape.x * blockSizeUnder;
	pos2.y += blockSizeUnder + _shape.y * blockSizeUnder;

	if (_color == BlockColor::red) {
		DrawFunc2D::DrawExtendGraph2D(pos1, pos2, blockTex[int(BlockColor::red)]);
	} else if (_color == BlockColor::blue) {
		DrawFunc2D::DrawExtendGraph2D(pos1, pos2, blockTex[int(BlockColor::blue)]);
	} else if (_color == BlockColor::yellow) {
		DrawFunc2D::DrawExtendGraph2D(pos1, pos2, blockTex[int(BlockColor::yellow)]);
	}
}

void Block::ActionDraw(const KuroEngine::Vec2<float> _pos, const BlockAttribute _attribute)
{
	const KuroEngine::Vec2<float> actionPos = { _pos.x + 50.0f,_pos.y - 10.0f };
	if (_attribute == BlockAttribute::attack1) {
		const KuroEngine::Vec2<float> actionSize = { 32.0f/2.0f,95.0f / 2.0f };
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ actionPos.x - actionSize.x / 2.0f,actionPos.y - actionSize.y / 2.0f },
			{ actionPos.x + actionSize.x / 2.0f,actionPos.y + actionSize.y / 2.0f }, actionTex[int(BlockAttribute::attack1)]);
	}else if (_attribute == BlockAttribute::attack2) {
		const KuroEngine::Vec2<float> actionSize = { 58.0f / 2.0f,84.0f / 2.0f };
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ actionPos.x - actionSize.x / 2.0f,actionPos.y - actionSize.y / 2.0f },
			{ actionPos.x + actionSize.x / 2.0f,actionPos.y + actionSize.y / 2.0f }, actionTex[int(BlockAttribute::attack2)]);
	} else if (_attribute == BlockAttribute::recovery) {
		const KuroEngine::Vec2<float> actionSize = { 68.0f / 2.0f,65.0f / 2.0f };
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ actionPos.x - actionSize.x / 2.0f,actionPos.y - actionSize.y / 2.0f },
			{ actionPos.x + actionSize.x / 2.0f,actionPos.y + actionSize.y / 2.0f }, actionTex[int(BlockAttribute::recovery)]);
	}
}
