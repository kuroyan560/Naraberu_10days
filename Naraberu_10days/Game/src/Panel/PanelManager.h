#pragma once
#include "../Block/BaseInformation.h"
#include "KuroEngineDevice.h"

class SetPrismEffect;

class PanelManager
{
public:

	struct BonusData {
		std::vector<KuroEngine::Vec2<int>> pos;
		BlockColor color;
		bool mass;
	};

	enum class Bonus {
		non,
		count,
		add,
	};

	PanelManager(){};
	~PanelManager(){};

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
	/// �ݒu�@by �n�V���g
	/// </summary>
	/// <param name="_setChipIdx">�͂߂�C���f�b�N�X</param>
	/// <param name="_color">�u���b�N�̐F</param>
	/// <returns>�ݒu�o������</returns>
	bool JudgeSet(KuroEngine::Vec2<int> _setChipIdx, BlockColor _color);

	/// <summary>
	/// Judge���ĉ��o�X�^�[�g�i�ݒu�͉��o�N���X�ɔC����jby �n�V���g
	/// </summary>
	/// <param name="_nowMapchip">�Z�b�g����I�u�W�F�N�g��mapchip</param>
	/// <param name="_shape">�Z�b�g����I�u�W�F�N�g�̌`��</param>
	/// <param name="_attribute">�A�N�V����</param>
	/// <param name="_color">�F</param>
	/// <param name="_playerAttackEffect">���o�N���X</param>
	/// <returns>���o�X�^�[�g������</returns>
	bool JudgeWithEffect(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
		const BlockAttribute _attribute, BlockColor _color, std::weak_ptr<SetPrismEffect>_playerAttackEffect);

	/// <summary>
	/// �򔻒�
	/// </summary>
	void MassProcess();

	/// <summary>
	/// �򔻒菈��
	/// </summary>
	/// <param name="_number">��ԍ�</param>
	/// <param name="_massNum">������</param>
	/// <param name="_massMap">���݂�mapchip�ԍ�</param>
	void MassBlock(const int _number, int* _massNum, const KuroEngine::Vec2<int> _massMap);

	/// <summary>
	/// �򔻒�
	/// </summary>
	void LineProcess();

	/// <summary>
	/// �򔻒菈��
	/// </summary>
	/// <param name="_number">��ԍ�</param>
	/// <param name="_lineMap">���݂�mapchip�ԍ�</param>
	/// <param name="_direction">���C���̌��� false->+y/true->+x</param>
	bool LineBlock(int _number, const KuroEngine::Vec2<int> _lineMap, const bool _direction);

	/// <summary>
	/// �{�[�i�X�̎Z�o
	/// </summary>
	void BonusCount();

	/// <summary>
	/// �{�[�i�X���o
	/// </summary>
	void BonusDirection();

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

	void SetBouns() { isBonusDirection = Bonus::count; }

private:
	
	//�{�[�i�X�����摜
	std::shared_ptr<KuroEngine::TextureBuffer> bonusTex;
	//�{�[�i�X!!!�摜
	std::shared_ptr<KuroEngine::TextureBuffer>bonusMarkTex;
	//�{�[�i�X�����摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,10> bonusNumberTex;
	//�X�e�[�W�̍ő�l
	static KuroEngine::Vec2<int> mapMax;
	//mapchip
	std::vector<std::vector<int>> mapchip;
	//��pmapchip�ϐ�
	std::vector<std::vector<int>> massMapchip;
	//��pmapchip�ϐ�
	std::vector<BonusData> bonusData;
	//�u���b�N�摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size)> blockTex;
	//�{�[�i�X�t���O
	Bonus isBonusDirection;
	//�{�[�i�X�����^�C�}�[
	float bonusTimer;
	//���݂̃{�[�i�X�������s���Ă���ԍ�
	int nowBonusNum;
	//�{�[�i�X�ύX�p�x
	float bonusAngle;
	//�{�[�i�X�ύX�傫��
	float bonusEaseScale;
	//�{�[�i�X�ύX�A���t�@
	float bonusAlpha;
	//�{�[�i�X���Z���̓���
	std::vector<KuroEngine::Vec2<int>> bonusPos;
};
