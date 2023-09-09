#include "PlayerAttackEffect.h"
#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"
#include"../Panel/PanelManager.h"

PlayerAttackEffect::PlayerAttackEffect()
{
	using namespace KuroEngine;

	//敵のダメージUIの位置
	const std::array<Vec2<float>, ENEMY_COUNT_MAX>ENEMY_DAMAGE_UI_POS =
	{
		Vec2<float>(700.0f,100.0f),
		Vec2<float>(700.0f,300.0f),
		Vec2<float>(700.0f,500.0f),
	};

	//敵のダメージUIの位置設定
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

		//全体攻撃
		if (m_targetEnemyIdx == ALL_ENEMY_ATTACK)
		{
			for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
				m_enemyDamageUI[enemyIdx].Add(m_damagePerOneBlock, true);
				//m_enemyDamageUI[enemyIdx].Add(m_damagePerOneBlock, enemyIdx == ENEMY_COUNT_MAX - 1);
		}
		//単体攻撃
		else
		{
			m_enemyDamageUI[m_targetEnemyIdx].Add(m_damagePerOneBlock, true);
		}

		if (!m_setChipIdxArray.empty())
			m_setBlockTimer.Reset();
	}

	//敵への与ダメージ演出終了したか
	int finishUiIdx = (m_targetEnemyIdx == ALL_ENEMY_ATTACK ? 0 : m_targetEnemyIdx);

	if (m_setChipIdxArray.empty() && !m_enemyDamageUI[finishUiIdx].GetIsActive())
	{
		m_isActive = false;

		// 設置したらアクション
		if (m_targetEnemyIdx != ALL_ENEMY_ATTACK) {
			// 弱攻撃
			PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", m_blockCount, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
		}
		else {
			// 強攻撃
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

	//ソート
	std::sort(m_setChipIdxArray.begin(), m_setChipIdxArray.end(), [](Vec2<int>a, Vec2<int>b) {
		if (a.x != b.x)return a.x > b.x;
		else return a.y > b.y;
		});

	//敵のダメージUIの位置
	const std::array<Vec2<float>, ENEMY_COUNT_MAX>ENEMY_DAMAGE_UI_POS =
	{
		Vec2<float>(949.0f,204.0f),
		Vec2<float>(949.0f,374.0f),
		Vec2<float>(949.0f,544.0f),
	};

	//敵のダメージUIの位置設定
	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].SetAppearPos(ENEMY_DAMAGE_UI_POS[enemyIdx]);
}