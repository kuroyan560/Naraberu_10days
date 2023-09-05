#include "BlockManager.h"

void BlockManager::Initialize()
{
	//形指定
	//後ほど読み込みに切り替え
	{
		canUseBlockType[0].texNum = 0;
		canUseBlockType[0].shape = { {0,0},{0,1},{0,-1},{1,0},{-1,0} };
		canUseBlockType[1].texNum = 1;
		canUseBlockType[1].shape = { {0,0},{0,-1},{0,-1},{0,-2} };
		canUseBlockType[2].texNum = 2;
		canUseBlockType[2].shape = { {0,0},{1,0},{1,1} };
		canUseBlockType[3].texNum = 3;
		canUseBlockType[3].shape = { {0,0},{-1,0},{0,1},{0,-1} };
	}

	block[int(ObjectType::use)].block.reset(new Block(true));
	block[int(ObjectType::use)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
	block[int(ObjectType::use)].blockNum = rand() % int(canUseBlockType.size());
	block[int(ObjectType::use)].color = BlockColor(rand() % int(BlockColor::size));
	block[int(ObjectType::use)].block->ChangeBlock(center, canUseBlockType[block[int(ObjectType::use)].blockNum].shape);

	for (int i = 1; i< int(ObjectType::size); i++) {
		block[i].block.reset(new Block());
		block[i].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
		block[i].blockNum = rand() % int(canUseBlockType.size());
		block[i].color = BlockColor(rand() % int(BlockColor::size));
	}
}

void BlockManager::Update()
{
	//ブロック配置後に次ブロックの移動を行う
	block[int(ObjectType::use)].block->Move();

	for (auto& i : block) {
		i.block->Update();
	}
}

void BlockManager::Draw()
{
	//移動処理を行っているブロック
	block[int(ObjectType::use)].block->Draw(canUseBlockType[block[int(ObjectType::use)].blockNum].shape,
		canUseBlockType[block[int(ObjectType::use)].blockNum].texNum, block[int(ObjectType::use)].color);
	//next表示のブロック
	block[int(ObjectType::next)].block->Draw(canUseBlockType[block[int(ObjectType::use)].blockNum].shape,
		canUseBlockType[block[int(ObjectType::next)].blockNum].texNum,
		block[int(ObjectType::next)].color, { 314.0f,628.0f });

	KuroEngine::Vec2<float> pos = { 450.0f,655.0f };
	for (int i = 2; i< int(ObjectType::size); i++) {
		block[i].block->Draw(canUseBlockType[block[int(ObjectType::use)].blockNum].shape,
			canUseBlockType[block[i].blockNum].texNum, block[i].color, pos);
		pos.x += 100.0f;
	}
}

void BlockManager::Reset()
{
}

void BlockManager::ChangeBlock()
{
	SetOneChangeBlock(int(ObjectType::use), int(ObjectType::next));
	SetOneChangeBlock(int(ObjectType::next), int(ObjectType::next1));
	SetOneChangeBlock(int(ObjectType::next1), int(ObjectType::next2));
	SetOneChangeBlock(int(ObjectType::next2), int(ObjectType::next3));

	block[int(ObjectType::next3)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
	block[int(ObjectType::next3)].blockNum = rand() % int(canUseBlockType.size());
	block[int(ObjectType::next3)].color = BlockColor(rand() % int(BlockColor::size));

	block[int(ObjectType::use)].block->ChangeBlock(center, canUseBlockType[block[int(ObjectType::use)].blockNum].shape);
}

void BlockManager::SetOneChangeBlock(const int a1, const int a2)
{
	block[a1].attribute = block[a2].attribute;
	block[a1].blockNum = block[a2].blockNum;
	block[a1].color = block[a2].color;
}
