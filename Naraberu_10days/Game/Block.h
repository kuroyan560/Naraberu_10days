#pragma once
#include "KuroEngineDevice.h"
#include"BaseInformation.h"

class Block
{
public:

	Block(bool _isUse = false);
	~Block(){};

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="_texNum">�摜�ԍ�</param>
	/// <param name="_color">�F</param>
	void Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const BlockAttribute _attribute, const BlockColor _color);

	/// <summary>
	/// ���W�L�̕`��
	/// </summary>
	/// <param name="_texNum">�摜�ԍ�</param>
	/// <param name="_color">�F</param>
	/// <param name="_pos">���W</param>
	void Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const KuroEngine::Vec2<float> shape_dist, const BlockAttribute _attribute, const BlockColor _color, const KuroEngine::Vec2<float>& _pos);

	/// <summary>
	/// ������
	/// </summary>
	void Reset();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �g�p����u���b�N���ς��ꍇ�̏���
	/// </summary>
	/// <param name="_mapchipNum">�^�񒆂�mapchip�ԍ�</param>
	/// <param name="_shape">�`��</param>
	void ChangeBlock(const KuroEngine::Vec2<int> _mapchipNum, const std::vector<KuroEngine::Vec2<int>> _shape);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="_color">�F</param>
	void BlockOneDraw(const KuroEngine::Vec2<float> pos, const BlockColor _color);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_shape">�`��</param>
	/// <param name="pos">���W</param>
	/// <param name="_color">�F</param>
	void BlockOneDraw(const KuroEngine::Vec2<int> _shape,
		const KuroEngine::Vec2<float> pos, const BlockColor _color);

	KuroEngine::Vec2<int> GetMapChipNum() { return pos; }

private:

	//�u���b�N�摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size)> blockTex;
	//���W(mapchip)
	KuroEngine::Vec2<int> pos;
	//�ړ��\��
	bool isMove;
	//�`��ŏ�
	KuroEngine::Vec2<int> shapeMin;
	//�`��ő�
	KuroEngine::Vec2<int> shapeMax;
};

