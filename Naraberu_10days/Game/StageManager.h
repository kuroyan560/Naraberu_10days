#pragma once
#include "BaseInformation.h"
#include "KuroEngineDevice.h"

class StageManager
{
public:

	StageManager(){};
	~StageManager(){};

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	
	/// <summary>
	/// 
	/// </summary>
	void Reset();

	/// <summary>
/// �Z�b�g����
	/// </summary>
	/// <param name="_nowMapchip">�Z�b�g����I�u�W�F�N�g��mapchip</param>
	/// <param name="_shape">�Z�b�g����I�u�W�F�N�g�̌`��</param>
	/// <param name="_attribute">�A�N�V����</param>
	/// <param name="_color">�F</param>
	/// <returns></returns>
	bool JudgeSet(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
		const BlockAttribute _attribute, BlockColor _color);

	/// <summary>
	/// �򔻒�
	/// </summary>
	/// <param name="_massNum">��̐�</param>
	/// <param name="_color">��̐F</param>
	bool MassProcess(std::vector<int>* _massNum, std::vector<BlockColor>* _color);

	/// <summary>
	/// �򔻒菈��
	/// </summary>
	/// <param name="_massNum">������</param>
	/// <param name="_massMap">���݂�mapchip�ԍ�</param>
	void MassBlock(int* _massNum, const KuroEngine::Vec2<int> _massMap);

	/// <summary>
	/// �򔻒�
	/// </summary>
	/// <param name="lineNum">���̐�</param>
	/// <param name="_color">���̐F</param>
	bool LineProcess(int* _lineNum, std::vector<BlockColor>* _color);

	/// <summary>
	/// �򔻒菈��
	/// </summary>
	/// <param name="_lineMap">���݂�mapchip�ԍ�</param>
	/// <param name="_direction">���C���̌��� false->+y/true->+x</param>
	int LineBlock(const KuroEngine::Vec2<int> _lineMap, const bool _direction);

	/// <summary>
	/// ���ז��Z�b�g
	/// </summary>
	/// <param name="_mapchip">�Z�b�g����}�b�v�`�b�v�ԍ�</param>
	void SetObstacle(const KuroEngine::Vec2<int> _mapchip) {
		mapchip[_mapchip.y][_mapchip.x] = 4;
	}

	/// <summary>
	/// �����o��
	/// </summary>
	/// <returns></returns>
	KuroEngine::Vec2<int> GetCenter() { return { int(mapchip.size()) / 2,int(mapchip[1].size()) / 2 }; }

	static KuroEngine::Vec2<int> GetMapMax(){return mapMax;}

	std::vector<std::vector<int>>* GetMapChipPtr() { return &mapchip; }

private:

	//�X�e�[�W�̍ő�l
	static KuroEngine::Vec2<int> mapMax;
	//mapchip
	std::vector<std::vector<int>> mapchip;
	//��pmapchip�ϐ�
	std::vector<std::vector<int>> massMapchip;
	//���C���pmapchip�ϐ�
	std::vector<std::vector<int>> lineMapchip;
	//�u���b�N�摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size)> blockTex;

};

