#pragma once
#include"../Block/BaseInformation.h"
#include"Common/Vec.h"
#include<vector>
#include"../UI/EnemyDamageUI.h"
#include<array>
#include"ForUser/Timer.h"
#include<memory>

class PanelManager;

//�U���̃u���b�N��u�����Ƃ��̉��o
class PlayerAttackEffect
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
	std::array<EnemyDamageUI, ENEMY_COUNT_MAX>m_enemyDamageUI;

	//�^�[�Q�b�g�̓G
	static const int ALL_ENEMY_ATTACK = -1;
	int m_targetEnemyIdx = ALL_ENEMY_ATTACK;

	//1�u���b�N�ɂ����_���[�W��
	int m_damagePerOneBlock = 1;

	//�P�u���b�N��u�����Ƃ̃^�C�}�[
	KuroEngine::Timer m_setBlockTimer;

public:
	PlayerAttackEffect();
	void Init();
	void Update(std::weak_ptr<PanelManager>arg_panelManager);
	void Draw();

	/// <summary>
	/// ���o�X�^�[�g�i�S�́j
	/// </summary>
	/// <param name="arg_setChipIdxArray">�ݒu�ʒu�C���f�b�N�X�̔z��</param>
	/// <param name="arg_color">�u���b�N�̐F</param>
	/// <param name="arg_damagePerOneBlock">1�u���b�N�ɂ����_���[�W��</param>
	/// <param name="arg_targetEnemy">����̓G�ɑ΂���U���Ȃ炻�̑Ώۂ͒N���i�f�t�H���g�͑S�̍U�������j</param>
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, int arg_targetEnemyIdx = ALL_ENEMY_ATTACK);

	//�A�N�e�B�u��ԃQ�b�^
	const bool& GetIsActive()const { return m_isActive; }
};