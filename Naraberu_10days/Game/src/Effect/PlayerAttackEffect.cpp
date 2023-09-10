#include "PlayerAttackEffect.h"
#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"
#include"../Panel/PanelManager.h"
#include"FrameWork/AudioApp.h"

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

	//カウントSE
	for (int i = 0; i < ATTACK_COUNT_SE_MAX; ++i)
	{
		std::string fileName = "attack_count_" + std::to_string(i) + ".wav";
		m_countSeArray[i] = AudioApp::Instance()->LoadAudio("resource/user/sound/attack_count/" + fileName, 0.5f);
	}
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
		for (auto& enemyIdx : m_targetEnemyIdxArray)
		{
			if (enemyIdx < 0)continue;
			if (ENEMY_COUNT_MAX <= enemyIdx)continue;

			m_enemyDamageUI[enemyIdx].Add(m_damagePerOneBlock, true);
		}

		//SE再生
		KuroEngine::AudioApp::Instance()->PlayWave(m_countSeArray[std::min(m_countSeIdx++, static_cast<int>(m_countSeArray.size()) - 1)]);

		if (!m_setChipIdxArray.empty())
			m_setBlockTimer.Reset();
	}

	//敵への与ダメージ演出終了したか
	int finishUiIdx = m_targetEnemyIdxArray[0];
	if (m_setChipIdxArray.empty() && !m_enemyDamageUI[finishUiIdx].GetIsActive())
	{
		m_isActive = false;

		// 設置したらアクション
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction(m_skillName, m_blockCount, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
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

void PlayerAttackEffect::Start(std::vector<KuroEngine::Vec2<int>> arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, std::vector<int>arg_targetEnemyIdxArray, std::string arg_skillName)
{
	using namespace KuroEngine;

	if (m_isActive)
	{
		assert(0);
	}

	m_isActive = true;
	m_targetEnemyIdxArray = arg_targetEnemyIdxArray;
	m_damagePerOneBlock = arg_damagePerOneBlock;
	m_setBlockTimer.Reset(5);
	m_color = arg_color;

	m_setChipIdxArray = arg_setChipIdxArray;
	m_blockCount = static_cast<int>(arg_setChipIdxArray.size());

	m_countSeIdx = 0;
	m_skillName = arg_skillName;

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