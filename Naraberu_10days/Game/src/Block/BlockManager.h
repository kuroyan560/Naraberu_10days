#pragma once
#include "../Block/BaseInformation.h"
#include "../Block/Block.h"
#include <memory>
#include <array>

class BlockManager
{
private:

	enum class ObjectType {
		use,//�g�p���Ă���u���b�N
		choice1,//�I���u���b�N1
		choice2,//�I���u���b�N2
		nextChoice1,//���̑I���u���b�N1
		nextChoice2,//���̑I���u���b�N2
		size,
	};
public:
	//���ۂɎg���Ă���u���b�N�̏��
	struct UseBlockType {
		int blockNum;
		BlockColor color;
		BlockAttribute attribute;
		std::shared_ptr<Block> block;
	};

	struct TutorialBlock {
		int blockNum;
		BlockColor color;
		BlockAttribute attribute;
	};

public:

	BlockManager() {};
	~BlockManager() {};

	void Initialize();

	void Update();

	void Draw();

	void Reset();

	/// <summary>
	/// �u���b�N�I��
	/// </summary>
	void ChoiceBlock();

	/// <summary>
	/// �u���b�N�̕ύX
	/// </summary>
	/// <returns></returns>
	void ChangeBlock();

	/// <summary>
	/// �����w��
	/// </summary>
	/// <param name="_center"></param>
	void SetCenter(const KuroEngine::Vec2<int> _center) { center = _center; }

	/// <summary>
	/// �z�u�u���b�N���Z�b�g
	/// </summary>
	/// <param name="_nowMapchip">���݂̃}�b�v�`�b�v�ԍ�</param>
	/// <param name="_shape">�`��</param>
	/// <param name="_color">�F</param>
	void GetBlock(KuroEngine::Vec2<int>* _nowMapchip, std::vector<KuroEngine::Vec2<int>>* _shape,
		BlockAttribute* _attribute, BlockColor* _color) {
		*_attribute = block[int(ObjectType::use)].attribute;
		*_color = block[int(ObjectType::use)].color;
		*_shape = shape[block[int(ObjectType::use)].blockNum];
		*_nowMapchip = block[int(ObjectType::use)].block->GetMapChipNum();
	}

	/// <summary>
	/// �u���b�N��ύX����ۂ̏���
	/// </summary>
	/// <param name="a1">�u���b�N�P</param>
	/// <param name="a2">�u���b�N�Q</param>
	void SetOneChangeBlock(const int a1, const int a2);

	// �p�X��
	void ResetPass() { passNum = passMaxNum; }

	//�`���[�g���A��
	void SetTutorial() { isTutorial = true; }

private:

	static const int tutorialMaxNum = 1;
	static const std::array<std::array<TutorialBlock, 10>, tutorialMaxNum>tutorialBlock;
	int tutorialNum;
	int tutorialBlockNum;
	//�`���[�g���A��
	bool isTutorial;
	//���摜
	std::shared_ptr<KuroEngine::TextureBuffer> arrowTex;
	//�p�X�摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> passTex;
	//����
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,3> numTex;
	//����
	KuroEngine::Vec2<int> center;
	//�g���u���b�N���
	std::array<UseBlockType, int(ObjectType::size)> block;
	//�I�𒆂̃u���b�N
	int nowChoice;
	//�p�X��
	const int passMaxNum=2;
	//�p�X�c���
	int passNum;
};

