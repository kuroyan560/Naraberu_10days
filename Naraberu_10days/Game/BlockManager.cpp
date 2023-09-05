#include "BlockManager.h"

void BlockManager::Initialize()
{
	block[int(ObjectType::use)].block.reset(new Block(true));
	block[int(ObjectType::use)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
	block[int(ObjectType::use)].blockNum = rand() % shapeNum;
	block[int(ObjectType::use)].color = BlockColor(rand() % int(BlockColor::size));
	block[int(ObjectType::use)].block->ChangeBlock(center, shape[block[int(ObjectType::use)].blockNum]);

	for (int i = 1; i< int(ObjectType::size); i++) {
		block[i].block.reset(new Block());
		block[i].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
		block[i].blockNum = rand() % shapeNum;
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
	float y = 633.0f;
	//移動処理を行っているブロック
	block[int(ObjectType::use)].block->Draw(shape[block[int(ObjectType::use)].blockNum],
		block[int(ObjectType::use)].attribute, block[int(ObjectType::use)].color);
	//choice1表示のブロック
	block[int(ObjectType::choice1)].block->Draw(shape[block[int(ObjectType::choice1)].blockNum],
		shape_dist[block[int(ObjectType::choice1)].blockNum],
		block[int(ObjectType::choice1)].attribute,
		block[int(ObjectType::choice1)].color, { 815.0f,y });
	//choice2表示のブロック
	block[int(ObjectType::choice2)].block->Draw(shape[block[int(ObjectType::choice2)].blockNum],
		shape_dist[block[int(ObjectType::choice2)].blockNum],
		block[int(ObjectType::choice2)].attribute,
		block[int(ObjectType::choice2)].color, { 710.0f,y });
	//nextChoice1表示のブロック
	block[int(ObjectType::nextChoice1)].block->Draw(shape[block[int(ObjectType::nextChoice1)].blockNum],
		shape_dist[block[int(ObjectType::nextChoice1)].blockNum],
		block[int(ObjectType::nextChoice1)].attribute,
		block[int(ObjectType::nextChoice1)].color, { 555.0f,y });
	//nextChoice2表示のブロック
	block[int(ObjectType::nextChoice2)].block->Draw(shape[block[int(ObjectType::nextChoice2)].blockNum],
		shape_dist[block[int(ObjectType::nextChoice2)].blockNum],
		block[int(ObjectType::nextChoice2)].attribute,
		block[int(ObjectType::nextChoice2)].color, { 445.0f,y });
}

void BlockManager::Reset()
{
}

void BlockManager::ChangeBlock()
{
	SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1));
	SetOneChangeBlock(int(ObjectType::choice1), int(ObjectType::choice2));
	SetOneChangeBlock(int(ObjectType::choice2), int(ObjectType::nextChoice1));
	SetOneChangeBlock(int(ObjectType::nextChoice1), int(ObjectType::nextChoice2));

	block[int(ObjectType::nextChoice2)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
	block[int(ObjectType::nextChoice2)].blockNum = rand() % shapeNum;
	block[int(ObjectType::nextChoice2)].color = BlockColor(rand() % int(BlockColor::size));

	block[int(ObjectType::use)].block->ChangeBlock(center, shape[block[int(ObjectType::use)].blockNum]);
}

void BlockManager::SetOneChangeBlock(const int a1, const int a2)
{
	block[a1].attribute = block[a2].attribute;
	block[a1].blockNum = block[a2].blockNum;
	block[a1].color = block[a2].color;
}
