#include "BlockManager.h"
#include "FrameWork/UsersInput.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"../OperationConfig.h"
#include"../SoundConfig.h"

#include "../Effect/ScreenShakeManager.h"

const std::array<std::array<BlockManager::TutorialBlock, 10>, 1>BlockManager::tutorialBlock =
{
	{
		{BlockManager::TutorialBlock(1, BlockColor::red,BlockAttribute::attack1),
		{1, BlockColor::red,BlockAttribute::attack1},{1, BlockColor::red,BlockAttribute::attack1},
		{1, BlockColor::red,BlockAttribute::attack1},{1, BlockColor::red,BlockAttribute::attack1},
		{1, BlockColor::red,BlockAttribute::attack1},{1, BlockColor::red,BlockAttribute::attack1},
		{1, BlockColor::red,BlockAttribute::attack1},{1, BlockColor::red,BlockAttribute::attack1},
		{1, BlockColor::red,BlockAttribute::attack1}}
	}
};

void BlockManager::Initialize()
{
	//画像
	std::string TexDir = "resource/user/tex/battle_scene/";
	arrowTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "arrow.png");
	passTex[0] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "pass.png");
	passTex[1] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "pass_slash.png");
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(numTex.data(), TexDir + "pass_number.png", 3, { 3, 1 });

	for (int i = 1; i< int(ObjectType::size); i++) {
		block[i].block.reset(new Block());
		block[i].attribute = tutorialBlock[tutorialNum][i].attribute;
		block[i].blockNum = tutorialBlock[tutorialNum][i].blockNum;
		block[i].color = tutorialBlock[tutorialNum][i].color;
	}
	tutorialBlockNum += 4;

	//使用ブロックをセット
	block[int(ObjectType::use)].block.reset(new Block(true));
	SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1));
	//使用ブロック変更処理
	block[int(ObjectType::use)].block->ChangeBlock(center, shape[block[int(ObjectType::use)].blockNum]);

	nowChoice = 0;

	passNum = passMaxNum;

	tutorialNum = 0;
	tutorialBlockNum = 0;
}

void BlockManager::Update()
{
	//パス
	if (passNum > 0 && OperationConfig::Instance()->GetOperationInput(OperationConfig::PASS_PAIR_PRISM,OperationConfig::ON_TRIGGER)) {
		ChangeBlock();
		passNum--;
	}

	//ブロック配置後に次ブロックの移動を行う
	block[int(ObjectType::use)].block->Move();
	//選択ブロック変更
	ChoiceBlock();

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
		block[int(ObjectType::choice1)].color, { 555.0f,y });
	//choice2表示のブロック
	block[int(ObjectType::choice2)].block->Draw(shape[block[int(ObjectType::choice2)].blockNum],
		shape_dist[block[int(ObjectType::choice2)].blockNum],
		block[int(ObjectType::choice2)].attribute,
		block[int(ObjectType::choice2)].color, { 445.0f,y });
	//nextChoice1表示のブロック
	block[int(ObjectType::nextChoice1)].block->Draw(shape[block[int(ObjectType::nextChoice1)].blockNum],
		shape_dist[block[int(ObjectType::nextChoice1)].blockNum],
		block[int(ObjectType::nextChoice1)].attribute,
		block[int(ObjectType::nextChoice1)].color, { 815.0f,y });
	//nextChoice2表示のブロック
	block[int(ObjectType::nextChoice2)].block->Draw(shape[block[int(ObjectType::nextChoice2)].blockNum],
		shape_dist[block[int(ObjectType::nextChoice2)].blockNum],
		block[int(ObjectType::nextChoice2)].attribute,
		block[int(ObjectType::nextChoice2)].color, { 710.0f,y });

	//矢印
	const KuroEngine::Vec2<float> arrowSize = { 39.0f,75.0f };
	if (nowChoice == 0) {
		KuroEngine::Vec2<float> arrowPos={ 585.0f,610.0f };
		arrowPos += ScreenShakeManager::Instance()->GetOffset();
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ arrowPos .x- arrowSize.x ,arrowPos .y- arrowSize.y }, arrowPos, arrowTex);
	}else if (nowChoice == 1) {
		KuroEngine::Vec2<float> arrowPos = { 475.0f,610.0f };
		arrowPos += ScreenShakeManager::Instance()->GetOffset();
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ arrowPos.x - arrowSize.x ,arrowPos.y - arrowSize.y }, arrowPos, arrowTex);
	}

	//パスの大本の座標
	const KuroEngine::Vec2<float> passFoundationPos={ 287.0f,668.0f };

	//パス画像
	const KuroEngine::Vec2<float> passSize = { 107.0f,27.0f };
	KuroEngine::DrawFunc2D::DrawExtendGraph2D(KuroEngine::Vec2(passFoundationPos.x - passSize.x, passFoundationPos.y - passSize.y) + ScreenShakeManager::Instance()->GetOffset(), passFoundationPos, passTex[0]);
	const KuroEngine::Vec2<float> passSlashSize = { 18.0f,27.0f };
	KuroEngine::Vec2<float> passSlashPos = { 54.0f,0.0f };
	passSlashPos += ScreenShakeManager::Instance()->GetOffset();
	KuroEngine::DrawFunc2D::DrawExtendGraph2D(
	{ passFoundationPos.x - passSlashSize.x + passSlashPos.x ,passFoundationPos.y - passSlashSize.y + passSlashPos.y },
	{ passFoundationPos.x + passSlashPos.x ,passFoundationPos.y + passSlashPos.y }, passTex[1]);

	//パス数字画像
	//最大
	KuroEngine::Vec2<float> passMaxNumPos = { 55.0f,-27.0f };
	passMaxNumPos += ScreenShakeManager::Instance()->GetOffset();
	KuroEngine::DrawFunc2D::DrawNumber2D(passMaxNum, { passFoundationPos.x + passMaxNumPos.x ,passFoundationPos.y + passMaxNumPos.y }, numTex.data(), {1,1});
	//現在の数
	KuroEngine::Vec2<float> passNumPos = { 8.0f,-27.0f };
	passNumPos += ScreenShakeManager::Instance()->GetOffset();
	KuroEngine::DrawFunc2D::DrawNumber2D(passNum, { passFoundationPos.x + passNumPos.x ,passFoundationPos.y + passNumPos.y }, numTex.data(), { 1,1 });

}

void BlockManager::Reset()
{
}

void BlockManager::ChoiceBlock()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	bool onKey = false;
	//選択移動
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::SELECT_RIGHT_PRISM, OperationConfig::ON_TRIGGER)) {
		if (nowChoice == 0) {
			nowChoice = 1;
		} else {
			nowChoice = 0;
		}
		onKey = true;
	} else if (OperationConfig::Instance()->GetOperationInput(OperationConfig::SELECT_LEFT_PRISM, OperationConfig::ON_TRIGGER)) {
		if (nowChoice == 1) {
			nowChoice = 0;
		} else {
			nowChoice = 1;
		}
		onKey = true;
	}

	if (onKey) {
		SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1) + nowChoice);
		//使用ブロック変更処理
		block[int(ObjectType::use)].block->ChangeBlock({ -1,-1 }, shape[block[int(ObjectType::use)].blockNum]);
		SoundConfig::Instance()->Play(SoundConfig::SE_SELECT_PRISM);
	}
}

void BlockManager::ChangeBlock()
{
	//次のブロック用意
	SetOneChangeBlock(int(ObjectType::choice1), int(ObjectType::nextChoice1));
	SetOneChangeBlock(int(ObjectType::choice2), int(ObjectType::nextChoice2));

	if (isTutorial) {
		if (tutorialBlockNum < 10) {
			block[int(ObjectType::nextChoice1)].attribute = tutorialBlock[tutorialNum][tutorialBlockNum].attribute;
			block[int(ObjectType::nextChoice1)].blockNum = tutorialBlock[tutorialNum][tutorialBlockNum].blockNum;
			block[int(ObjectType::nextChoice1)].color = tutorialBlock[tutorialNum][tutorialBlockNum].color;
			block[int(ObjectType::nextChoice2)].attribute = tutorialBlock[tutorialNum][tutorialBlockNum + 1].attribute;
			block[int(ObjectType::nextChoice2)].blockNum = tutorialBlock[tutorialNum][tutorialBlockNum + 1].blockNum;
			block[int(ObjectType::nextChoice2)].color = tutorialBlock[tutorialNum][tutorialBlockNum + 1].color;
			tutorialBlockNum += 2;
		}
		//セーフティ―
		else {
			block[int(ObjectType::nextChoice1)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
			block[int(ObjectType::nextChoice1)].blockNum = rand() % shapeNum;
			block[int(ObjectType::nextChoice1)].color = BlockColor(rand() % (int(BlockColor::size) - 3));
			block[int(ObjectType::nextChoice2)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
			block[int(ObjectType::nextChoice2)].blockNum = rand() % shapeNum;
			block[int(ObjectType::nextChoice2)].color = BlockColor(rand() % (int(BlockColor::size) - 3));
		}
	} else {
		block[int(ObjectType::nextChoice1)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
		block[int(ObjectType::nextChoice1)].blockNum = rand() % shapeNum;
		block[int(ObjectType::nextChoice1)].color = BlockColor(rand() % (int(BlockColor::size) - 3));
		block[int(ObjectType::nextChoice2)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
		block[int(ObjectType::nextChoice2)].blockNum = rand() % shapeNum;
		block[int(ObjectType::nextChoice2)].color = BlockColor(rand() % (int(BlockColor::size) - 3));
	}
	//使用ブロックをセット
	SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1));
	//使用ブロック変更処理
	block[int(ObjectType::use)].block->ChangeBlock(center, shape[block[int(ObjectType::use)].blockNum]);
	//choice変更
	nowChoice = 0;
}

void BlockManager::SetOneChangeBlock(const int a1, const int a2)
{
	block[a1].attribute = block[a2].attribute;
	block[a1].blockNum = block[a2].blockNum;
	block[a1].color = block[a2].color;
}
