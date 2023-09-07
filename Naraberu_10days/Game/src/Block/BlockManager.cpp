#include "BlockManager.h"
#include "FrameWork/UsersInput.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"../OperationConfig.h"

void BlockManager::Initialize()
{
	//�摜
	std::string TexDir = "resource/user/tex/battle_scene/";
	arrowTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "arrow.png");

	for (int i = 1; i< int(ObjectType::size); i++) {
		block[i].block.reset(new Block());
		block[i].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
		block[i].blockNum = rand() % shapeNum;
		block[i].color = BlockColor(rand() % (int(BlockColor::size) - 3));
	}

	//�g�p�u���b�N���Z�b�g
	block[int(ObjectType::use)].block.reset(new Block(true));
	SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1));
	//�g�p�u���b�N�ύX����
	block[int(ObjectType::use)].block->ChangeBlock(center, shape[block[int(ObjectType::use)].blockNum]);

	nowChoice = 0;
	isEnemyAttack=false;
}

void BlockManager::Update()
{
	if (!isEnemyAttack) {
		//�p�X
		if (OperationConfig::Instance()->DebugKeyInputOnTrigger(DIK_P)) {
			ChangeBlock();
		}

		//�u���b�N�z�u��Ɏ��u���b�N�̈ړ����s��
		block[int(ObjectType::use)].block->Move();
		//�I���u���b�N�ύX
		ChoiceBlock();
	}

	for (auto& i : block) {
		i.block->Update();
	}
}

void BlockManager::Draw()
{
	float y = 633.0f;
	//�ړ��������s���Ă���u���b�N
	block[int(ObjectType::use)].block->Draw(shape[block[int(ObjectType::use)].blockNum],
		block[int(ObjectType::use)].attribute, block[int(ObjectType::use)].color);
	//choice1�\���̃u���b�N
	block[int(ObjectType::choice1)].block->Draw(shape[block[int(ObjectType::choice1)].blockNum],
		shape_dist[block[int(ObjectType::choice1)].blockNum],
		block[int(ObjectType::choice1)].attribute,
		block[int(ObjectType::choice1)].color, { 555.0f,y });
	//choice2�\���̃u���b�N
	block[int(ObjectType::choice2)].block->Draw(shape[block[int(ObjectType::choice2)].blockNum],
		shape_dist[block[int(ObjectType::choice2)].blockNum],
		block[int(ObjectType::choice2)].attribute,
		block[int(ObjectType::choice2)].color, { 445.0f,y });
	//nextChoice1�\���̃u���b�N
	block[int(ObjectType::nextChoice1)].block->Draw(shape[block[int(ObjectType::nextChoice1)].blockNum],
		shape_dist[block[int(ObjectType::nextChoice1)].blockNum],
		block[int(ObjectType::nextChoice1)].attribute,
		block[int(ObjectType::nextChoice1)].color, { 815.0f,y });
	//nextChoice2�\���̃u���b�N
	block[int(ObjectType::nextChoice2)].block->Draw(shape[block[int(ObjectType::nextChoice2)].blockNum],
		shape_dist[block[int(ObjectType::nextChoice2)].blockNum],
		block[int(ObjectType::nextChoice2)].attribute,
		block[int(ObjectType::nextChoice2)].color, { 710.0f,y });

	//���
	const KuroEngine::Vec2<float> arrowSize = { 39.0f,75.0f };
	if (nowChoice == 0) {
		KuroEngine::Vec2<float> arrowPos={ 585.0f,610.0f };
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ arrowPos .x- arrowSize.x ,arrowPos .y- arrowSize.y }, arrowPos, arrowTex);
	}else if (nowChoice == 1) {
		KuroEngine::Vec2<float> arrowPos = { 475.0f,610.0f };
		KuroEngine::DrawFunc2D::DrawExtendGraph2D({ arrowPos.x - arrowSize.x ,arrowPos.y - arrowSize.y }, arrowPos, arrowTex);
	}
}

void BlockManager::Reset()
{
}

void BlockManager::ChoiceBlock()
{
	KuroEngine::UsersInput* input = KuroEngine::UsersInput::Instance();

	//�I���ړ�
	if (OperationConfig::Instance()->GetOperationInput(OperationConfig::SELECT_RIGHT_PRISM, OperationConfig::ON_TRIGGER)) {
		nowChoice = 0;
		SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1));
		//�g�p�u���b�N�ύX����
		block[int(ObjectType::use)].block->ChangeBlock({ -1,-1 }, shape[block[int(ObjectType::use)].blockNum]);
	} else if (OperationConfig::Instance()->GetOperationInput(OperationConfig::SELECT_LEFT_PRISM, OperationConfig::ON_TRIGGER)) {
		nowChoice = 1;
		SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice2));
		//�g�p�u���b�N�ύX����
		block[int(ObjectType::use)].block->ChangeBlock({ -1,-1 }, shape[block[int(ObjectType::use)].blockNum]);
	}

	input = nullptr;
}

void BlockManager::ChangeBlock()
{
	//���̃u���b�N�p��
	SetOneChangeBlock(int(ObjectType::choice1), int(ObjectType::nextChoice1));
	SetOneChangeBlock(int(ObjectType::choice2), int(ObjectType::nextChoice2));

	block[int(ObjectType::nextChoice1)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
	block[int(ObjectType::nextChoice1)].blockNum = rand() % shapeNum;
	block[int(ObjectType::nextChoice1)].color = BlockColor(rand() % (int(BlockColor::size) - 3));
	block[int(ObjectType::nextChoice2)].attribute = BlockAttribute(rand() % int(BlockAttribute::size));
	block[int(ObjectType::nextChoice2)].blockNum = rand() % shapeNum;
	block[int(ObjectType::nextChoice2)].color = BlockColor(rand() % (int(BlockColor::size) - 3));

	//�g�p�u���b�N���Z�b�g
	SetOneChangeBlock(int(ObjectType::use), int(ObjectType::choice1));
	//�g�p�u���b�N�ύX����
	block[int(ObjectType::use)].block->ChangeBlock(center, shape[block[int(ObjectType::use)].blockNum]);
	//choice�ύX
	nowChoice = 0;
}

void BlockManager::SetOneChangeBlock(const int a1, const int a2)
{
	block[a1].attribute = block[a2].attribute;
	block[a1].blockNum = block[a2].blockNum;
	block[a1].color = block[a2].color;
}
