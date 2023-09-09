#include "PlayerAttackEffect.h"
#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"
#include"../Panel/PanelManager.h"

PlayerAttackEffect::PlayerAttackEffect()
{
	using namespace KuroEngine;

	//�G�̃_���[�WUI�̈ʒu
	const std::array<Vec2<float>, ENEMY_COUNT_MAX>ENEMY_DAMAGE_UI_POS =
	{
		Vec2<float>(700.0f,100.0f),
		Vec2<float>(700.0f,300.0f),
		Vec2<float>(700.0f,500.0f),
	};

	//�G�̃_���[�WUI�̈ʒu�ݒ�
	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].SetAppearPos(ENEMY_DAMAGE_UI_POS[enemyIdx]);
}

void PlayerAttackEffect::Init()
{
	using namespace KuroEngine;

	m_isActive = false;

	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].Init();

}

void PlayerAttackEffect::Update(std::weak_ptr<PanelManager>arg_panelManager)
{
	if (!m_isActive)return;

	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].Update();

	m_setBlockTimer.UpdateTimer();
	if (m_setBlockTimer.IsTimeUpOnTrigger())
	{
		arg_panelManager.lock()->JudgeSet(m_setChipIdxArray.back(), m_color);
		m_setChipIdxArray.pop_back();

		//�S�̍U��
		if (m_targetEnemyIdx == ALL_ENEMY_ATTACK)
		{
			for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
				m_enemyDamageUI[enemyIdx].Add(m_damagePerOneBlock, true);
				//m_enemyDamageUI[enemyIdx].Add(m_damagePerOneBlock, enemyIdx == ENEMY_COUNT_MAX - 1);
		}
		//�P�̍U��
		else
		{
			m_enemyDamageUI[m_targetEnemyIdx].Add(m_damagePerOneBlock, true);
		}

		if (!m_setChipIdxArray.empty())
			m_setBlockTimer.Reset();
	}

	//�G�ւ̗^�_���[�W���o�I��������
	int finishUiIdx = (m_targetEnemyIdx == ALL_ENEMY_ATTACK ? 0 : m_targetEnemyIdx);

	if (m_setChipIdxArray.empty() && !m_enemyDamageUI[finishUiIdx].GetIsActive())
	{
		m_isActive = false;

		// �ݒu������A�N�V����
		if (m_targetEnemyIdx != ALL_ENEMY_ATTACK) {
			// ��U��
			PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", m_blockCount, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
		}
		else {
			// ���U��
			PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_02", m_blockCount, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
		}
	}
}

void PlayerAttackEffect::Draw()
{
	if (!m_isActive)return;

	for (auto& ui : m_enemyDamageUI)
	{
		ui.Draw();
	}
}

void PlayerAttackEffect::Start(std::vector<KuroEngine::Vec2<int>> arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, int arg_targetEnemyIdx)
{
	using namespace KuroEngine;

	if (m_isActive)
	{
		assert(0);
	}

	m_isActive = true;
	m_targetEnemyIdx = arg_targetEnemyIdx;
	m_damagePerOneBlock = arg_damagePerOneBlock;
	m_setBlockTimer.Reset(5);
	m_color = arg_color;

	m_setChipIdxArray = arg_setChipIdxArray;
	m_blockCount = static_cast<int>(arg_setChipIdxArray.size());

	//�\�[�g
	std::sort(m_setChipIdxArray.begin(), m_setChipIdxArray.end(), [](Vec2<int>a, Vec2<int>b) {
		if (a.x != b.x)return a.x > b.x;
		else return a.y > b.y;
		});

	//�G�̃_���[�WUI�̈ʒu
	const std::array<Vec2<float>, ENEMY_COUNT_MAX>ENEMY_DAMAGE_UI_POS =
	{
		Vec2<float>(949.0f,204.0f),
		Vec2<float>(949.0f,374.0f),
		Vec2<float>(949.0f,544.0f),
	};

	//�G�̃_���[�WUI�̈ʒu�ݒ�
	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].SetAppearPos(ENEMY_DAMAGE_UI_POS[enemyIdx]);
}