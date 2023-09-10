#pragma once
#include"../Block/BaseInformation.h"
#include"Common/Vec.h"
#include<vector>
#include"../UI/SkillResultUI.h"
#include<array>
#include"ForUser/Timer.h"
#include<memory>
#include<string>

class PanelManager;
class ParticleEmitter;

//�v���Y����u�����Ƃ��̉��o
class SetPrismEffect
{
	bool m_isActive = false;

	//�ݒu�ʒu�C���f�b�N�X�̔z��
	std::vector<KuroEngine::Vec2<int>>m_setChipIdxArray;

	//�ݒu��
	int m_blockCount = 0;

	//�u���b�N�̐F
	BlockColor m_color;

	//�G�ɑ΂���^�_���[�WUI
	static const int ENEMY_COUNT_MAX = 3;
	std::array<SkillResultUI, ENEMY_COUNT_MAX>m_enemyDamageUI;

	//�v���C���[�ɑ΂����UI
	SkillResultUI m_playerHealUI;

	//�N������UI
	std::vector<SkillResultUI*>m_activeUIArray;

	//�^�[�Q�b�g�̓G
	std::vector<int>m_targetEnemyIdxArray;

	//1�u���b�N�ɂ��ǂꂭ�炢�̌��ʂ����邩
	int m_amountPerOneBlock = 1;

	//�P�u���b�N��u�����Ƃ̃^�C�}�[
	KuroEngine::Timer m_setPrismTimer;

	//�U���J�E���g�ő�
	static const int SET_PRISM_COUNT_SE_MAX = 10;
	//�U���J�E���gSE
	std::array<int, SET_PRISM_COUNT_SE_MAX>m_countSeArray;
	//�Đ�����J�E���gSE�̃C���f�b�N�X
	int m_countSeIdx;

	std::string m_skillName;

	//���ʂ̃X�^�[�g���̏�����
	void CommonInitOnStart(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_amountPerOneBlock, std::string arg_skillName);

public:
	SetPrismEffect();
	void Init();
	void Update(std::weak_ptr<PanelManager>arg_panelManager, std::weak_ptr<ParticleEmitter>arg_ultParticleEmitter);
	void Draw();

	/// <summary>
	/// ���o�X�^�[�g�i�S�́j
	/// </summary>
	/// <param name="arg_setChipIdxArray">�ݒu�ʒu�C���f�b�N�X�̔z��</param>
	/// <param name="arg_color">�u���b�N�̐F</param>
	/// <param name="arg_damagePerOneBlock">1�u���b�N�ɂ����_���[�W��</param>
	/// <param name="arg_skillName">�X�L����</param>
	/// <param name="arg_targetEnemyIdxArray">�^�[�Q�b�g�̓G�̃C���f�b�N�X�z��</param>
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, std::string arg_skillName, std::vector<int>arg_targetEnemyIdxArray);
	
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_healPerOneBlock, std::string arg_skillName);

	//�A�N�e�B�u��ԃQ�b�^
	const bool& GetIsActive()const { return m_isActive; }
};