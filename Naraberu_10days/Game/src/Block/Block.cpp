#include "Block.h"
#include "../../src/engine/DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "FrameWork/UsersInput.h"
#include "BaseInformation.h"
#include "../Panel/PanelManager.h"
#include"../OperationConfig.h"
#include"../SoundConfig.h"

#include "../Effect/ScreenShakeManager.h"
#include"../BattleManager/ExistUnits.h"

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

	if (ExistUnits::Instance()->m_mapSize==8) {
		blockSize = blockSize8x8;
	} else {
		blockSize = blockSize10x10;
	}
}

void Block::Update()
{
}

void Block::Draw(bool _isSelect, const std::vector<KuroEngine::Vec2<int>> _shape, const BlockAttribute _attribute, const BlockColor _color, const float _rota, const float _alpha, const float _scale)
{
	for (auto& i : _shape) {
		KuroEngine::Vec2<float> inpos = { (pos.x + i.x) * blockSize + difference.x , (pos.y + i.y) * blockSize + difference.y };
		inpos += ScreenShakeManager::Instance()->GetOffset();
		BlockOneDraw(inpos, _color, _rota,
			_isSelect ? ExistUnits::Instance()->m_StageManager->CanSetBlock(pos + i) : false, _alpha, _scale);
	}

	ActionDraw({ (pos.x + shapeMax.x) * blockSize + difference.x, (pos.y + shapeMin.y) * blockSize + difference.y }, _attribute, _alpha);
}

void Block::Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const KuroEngine::Vec2<float> shape_dist,
	BlockAttribute _attribute, const BlockColor _color, const KuroEngine::Vec2<float>& _pos, const float _alpha)
{
	KuroEngine::Vec2<float> pos = { _pos.x - shape_dist.x,_pos.y - shape_dist.y };
	pos += ScreenShakeManager::Instance()->GetOffset();
	for (auto& i : _shape) {
		BlockOneDraw(i, pos, _color, _alpha);
	}

	ActionDraw({ shapeMax.x * blockSize + _pos.x, shapeMin.y * blockSize + _pos.y }, _attribute, _alpha);
}

void Block::Reset()
{
	
}

KuroEngine::Vec2<int> Block::Move()
{
	KuroEngine::Vec2<int> max = PanelManager::GetMapMax();
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();
	auto before = pos;
	if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_LEFT)) {
		if (pos.x <= -shapeMin.x) { return { 0,0 }; }
		pos.x -= 1;
	} else if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_RIGHT)) {
		if (pos.x >= max.x - 1 - shapeMax.x) { return { 0,0 }; }
		pos.x += 1;
	} else if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_UP)) {
		if (pos.y <= -shapeMin.y) { return { 0,0 }; }
		pos.y -= 1;
	} else if (OperationConfig::Instance()->GetMoveVec(OperationConfig::SELECT_VEC_DOWN)) {
		if (pos.y >= max.y - 1 - shapeMax.y) { return { 0,0 }; }
		pos.y += 1;
	}

	if (before != pos)
	{
		SoundConfig::Instance()->Play(SoundConfig::SE_SELECT);
	}

	return pos - before;
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
	}

	KuroEngine::Vec2<int> max = PanelManager::GetMapMax();

	//ìÀÇ´èoÇƒÇ¢ÇΩèÍçáñﬂÇ∑
	pos.x += int(pos.x + shapeMin.x < 0) * abs(pos.x + shapeMin.x);
	pos.x -= int(shapeMax.x + pos.x >= max.x) * (shapeMax.x + pos.x - max.x + 1);
	pos.y += int(pos.y + shapeMin.y < 0) * abs(pos.y + shapeMin.y);
	pos.y -= int(shapeMax.y + pos.y >= max.y) * (shapeMax.y + pos.y - max.y + 1);
}

void Block::BlockOneDraw(const KuroEngine::Vec2<float> _pos, BlockColor _color, const float _rota, const bool _canPut, const float _alpha, const float _scale)
{
	using namespace KuroEngine;

	Vec2<float> pos1 = _pos;
	pos1.x += blockSize / 2.0f;
	pos1.y += blockSize / 2.0f;

	pos1 += ScreenShakeManager::Instance()->GetOffset();

	float rota = _rota * (3.14f / 180.0f);
	auto tex = _canPut ? blockTex[int(_color)] : lineTex[int(_color)];
	float alpha = _canPut ? _alpha : 1.0f;

	DrawFunc2D::DrawRotaGraph2D(pos1, { blockSize / 64.0f * _scale,blockSize / 64.0f * _scale }, rota, tex, alpha);
}

void Block::BlockOneDraw(const KuroEngine::Vec2<int> _shape, const KuroEngine::Vec2<float> pos, const BlockColor _color,const float _alpha)
{
	using namespace KuroEngine;
	const float blockSizeUnder = 20.0f;

	Vec2<float> pos1 = pos;
	pos1.x += _shape.x * blockSizeUnder;
	pos1.y += _shape.y * blockSizeUnder;

	Vec2<float> pos2 = pos;
	pos2.x += blockSizeUnder + _shape.x * blockSizeUnder;
	pos2.y += blockSizeUnder + _shape.y * blockSizeUnder;

	pos1 += ScreenShakeManager::Instance()->GetOffset();
	pos2 += ScreenShakeManager::Instance()->GetOffset();

	if (_color == BlockColor::red) {
		DrawFunc2D::DrawExtendGraph2D(pos1, pos2, blockTex[int(BlockColor::red)],_alpha);
	} else if (_color == BlockColor::blue) {
		DrawFunc2D::DrawExtendGraph2D(pos1, pos2, blockTex[int(BlockColor::blue)],_alpha);
	} else if (_color == BlockColor::yellow) {
		DrawFunc2D::DrawExtendGraph2D(pos1, pos2, blockTex[int(BlockColor::yellow)], _alpha);
	}
}

void Block::ActionDraw(const KuroEngine::Vec2<float> _pos, const BlockAttribute _attribute, const float _alpha)
{
	KuroEngine::Vec2<float> actionPos = { _pos.x + 50.0f,_pos.y - 10.0f };

	actionPos += ScreenShakeManager::Instance()->GetOffset();

	KuroEngine::DrawFunc2D::DrawRotaGraph2D(actionPos, { 0.5f,0.5f }, 0.0f, actionTex[int(_attribute)], _alpha);
}
