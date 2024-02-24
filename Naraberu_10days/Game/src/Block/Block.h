#pragma once
#include "KuroEngineDevice.h"
#include "../Block/BaseInformation.h"

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
	void Draw(bool _isSelect,
		const std::vector<KuroEngine::Vec2<int>> _shape, const BlockAttribute _attribute, const BlockColor _color, const float _rota = 0.0f, const float _alpha = 1.0f);

	/// <summary>
	/// ���W�L�̕`��
	/// </summary>
	/// <param name="_texNum">�摜�ԍ�</param>
	/// <param name="_color">�F</param>
	/// <param name="_pos">���W</param>
	void Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const KuroEngine::Vec2<float> shape_dist, const BlockAttribute _attribute, const BlockColor _color, const KuroEngine::Vec2<float>& _pos,const float _alpha);

	/// <summary>
	/// ������
	/// </summary>
	void Reset();

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <returns>�ړ���</returns>
	KuroEngine::Vec2<int>Move();

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
	void BlockOneDraw(const KuroEngine::Vec2<float> pos, const BlockColor _color, const float _rota, const bool _canPut, const float _alpha);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_shape">�`��</param>
	/// <param name="pos">���W</param>
	/// <param name="_color">�F</param>
	void BlockOneDraw(const KuroEngine::Vec2<int> _shape,
		const KuroEngine::Vec2<float> pos, const BlockColor _color,const float _alpha);

	/// <summary>
	/// action�`��
	/// </summary>
	/// <param name="_pos">���W</param>
	/// <param name="_attribute">�A�N�V����</param>
	void ActionDraw(const KuroEngine::Vec2<float> _pos, const BlockAttribute _attribute, const float _alpha);

	KuroEngine::Vec2<int> GetMapChipNum() { return pos; }

private:

	//�u���b�N�摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size) - 2> blockTex;
	//�ː��u���b�N
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size) - 1> lineTex;
	//�A�N�V�����摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockAttribute::size)> actionTex;
	//���W(mapchip)
	KuroEngine::Vec2<int> pos;
	//�ړ��\��
	bool isMove;
	//�`��ŏ�
	KuroEngine::Vec2<int> shapeMin;
	//�`��ő�
	KuroEngine::Vec2<int> shapeMax;

	float blockSize;
};

