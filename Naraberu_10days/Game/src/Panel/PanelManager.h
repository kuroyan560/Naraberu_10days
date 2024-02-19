#pragma once
#include "../Block/BaseInformation.h"
#include "KuroEngineDevice.h"
#include<vector>
#include<memory>

class SetPrismEffect;
class SkillResultUI;
class PerfectBonusEffect;

class PanelManager
{
public:

	enum class BonusKind {
		color,
		line,
		gold,
		size,
	};

	struct BonusData {
		BonusKind bonusKind;
		std::vector<KuroEngine::Vec2<int>> pos;
		BlockColor color;
		bool mass;
	};

	enum class Bonus {
		non,
		count,
		add,
		perfect,
	};

	enum class PanelSize {
		_8x8,
		_10x10
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
	void Update(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void BonusDraw();
	
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
	/// ������
	/// </summary>
	int GoldProcess();

	/// <summary>
	/// �{�[�i�X�̎Z�o
	/// </summary>
	void BonusCount();

	/// <summary>
	/// �{�[�i�X���o
	/// </summary>
	void BonusDirection(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect);

	/// <summary>
	/// �p�[�t�F�N�g���o
	/// </summary>
	void PerfectBonus(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect);

	/// <summary>
	/// ��݂̂̋󂫂𔻒�
	/// </summary>
	void OneProcess();

	/// <summary>
	/// �T��
	/// </summary>
	/// <param name="_massNum">��</param>
	/// <param name="_mapchip">�ԍ�</param>
	void OneBlock(int* _massNum, const KuroEngine::Vec2<int> _mapchip);

	/// <summary>
	/// �u���b�N�Z�b�g
	/// </summary>
	/// <param name="_mapchip">�Z�b�g����}�b�v�`�b�v�ԍ�</param>
	void SetBlock(const KuroEngine::Vec2<int> _mapchip,int _color) {
		mapchip[_mapchip.y][_mapchip.x] = _color;
	}

	/// <summary>
	/// ���ז��Z�b�g
	/// </summary>
	/// <param name="_mapchip">�Z�b�g����}�b�v�`�b�v�ԍ�</param>
	void SetObstacle(const KuroEngine::Vec2<int> _mapchip) {
		mapchip[_mapchip.y][_mapchip.x] = 4;
	}


	/// <summary>
	/// �w�肵���}�b�v�`�b�v�̕`�撆�S���W�����߂�
	/// </summary>
	/// <param name="_mapchip">���S���W�����߂����}�b�v�`�b�v</param>
	/// <returns>�w�肵���}�b�v�`�b�v�̕`�撆�S���W</returns>
	KuroEngine::Vec2<float> GetChipCenterPos(const KuroEngine::Vec2<int>_mapchip);

	/// <summary>
	/// �P�����̋󂫂̃}�b�v�`�b�v�z��Q�b�^
	/// </summary>
	/// <returns>�P�����̋󂫂̃}�b�v�`�b�v�z��</returns>
	const std::vector<KuroEngine::Vec2<int>>& GetOneSpacePosArray() { return onePos; }

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void SetGold();

	/// <summary>
	/// �����o��
	/// </summary>
	/// <returns></returns>
	KuroEngine::Vec2<int> GetCenter() { return { int(mapchip.size()) / 2,int(mapchip[1].size()) / 2 }; }

	static KuroEngine::Vec2<int> GetMapMax(){return mapMax;}

	std::vector<std::vector<int>>* GetMapChipPtr() { return &mapchip; }

	void SetBouns() { isBonusDirection = Bonus::count; }

	bool IsPerfect()const;

	/// <summary>
	/// �w�肵���`�b�v�C���f�b�N�X�Ƀu���b�N��u���邩
	/// </summary>
	/// <param name="_chip">�}�b�v�`�b�v�C���f�b�N�X</param>
	/// <returns>�u���b�N��u���邩</returns>
	bool CanSetBlock(KuroEngine::Vec2<int>_chip)const
	{
		return mapchip[_chip.y][_chip.x] == int(BlockColor::yuka);
	}

private:
	
	//panel�̑傫��
	PanelSize panelSize;
	//block��̑傫��
	float blockSize;
	//�{�[�i�X��ޕ����摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BonusKind::size)> bonusKindTex;
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
	//��݂̂̔���p
	std::vector<std::vector<int>> oneMapchip;
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
	//������̋󂫊i�[
	std::vector<KuroEngine::Vec2<int>> onePos;
	//�{�[�i�X�̍��v
	int totalBounsNum;
public:
	int GetTotalBonus() { return totalBounsNum; }
};
