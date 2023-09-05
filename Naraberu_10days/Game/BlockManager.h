#pragma once
#include "BaseInformation.h"
#include "Block.h"
#include <memory>
#include <array>

class BlockManager
{
private:

	enum class ObjectType {
		use,//�g�p���Ă���u���b�N
		next,//next�\���̃u���b�N
		next1,//1�Ԗڃu���b�N
		next2,//2�Ԗڃu���b�N
		next3,//3�Ԗڃu���b�N
		size,
	};

	//�g�p�ł���u���b�N�^�C�v
	struct CanUseBlockType {
		int texNum;
		std::vector<KuroEngine::Vec2<int>> shape;//�`��
	};

	//���ۂɎg���Ă���u���b�N�̏��
	struct UseBlockType {
		int blockNum;
		BlockColor color;
		BlockAttribute attribute;
		std::shared_ptr<Block> block;
	};

public:

	BlockManager() {};
	~BlockManager() {};

	void Initialize();

	void Update();

	void Draw();

	void Reset();

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
		*_shape = canUseBlockType[block[int(ObjectType::use)].blockNum].shape;
		*_nowMapchip = block[int(ObjectType::use)].block->GetMapChipNum();
	}

	/// <summary>
	/// �u���b�N��ύX����ۂ̏���
	/// </summary>
	/// <param name="a1">�u���b�N�P</param>
	/// <param name="a2">�u���b�N�Q</param>
	void SetOneChangeBlock(const int a1, const int a2);

private:

	//����
	KuroEngine::Vec2<int> center;
	//�g����u���b�N�̎��
	std::array<CanUseBlockType,4> canUseBlockType;
	//�g���u���b�N���
	std::array<UseBlockType, int(ObjectType::size)> block;
};

