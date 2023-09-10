#include "SetPrismEffect.h"
#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"
#include"../Panel/PanelManager.h"
#include"FrameWork/AudioApp.h"

void SetPrismEffect::CommonInitOnStart(std::vector<KuroEngine::Vec2<int>> arg_setChipIdxArray, BlockColor arg_color, int arg_amountPerOneBlock, std::string arg_skillName)
{
	using namespace KuroEngine;

	if (m_isActive)
	{
		assert(0);
	}

	m_isActive = true;
	m_amountPerOneBlock = arg_amountPerOneBlock;
	m_setPrismTimer.Reset(5);
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
}

SetPrismEffect::SetPrismEffect()
{
	using namespace KuroEngine;

	//敵のダメージUIの位置
	const std::array<Vec2<float>, ENEMY_COUNT_MAX>ENEMY_DAMAGE_UI_POS =
	{
		Vec2<float>(949.0f,204.0f),
		Vec2<float>(949.0f,374.0f),
		Vec2<float>(949.0f,544.0f),
	};

	//敵のダメージUIの位置設定
	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].Set(SkillResultUI::SKILL_ENEMY_DAMAGE, ENEMY_DAMAGE_UI_POS[enemyIdx], 1500.0f);

	//プレイヤーの回復UIの位置設定
	m_playerHealUI.Set(SkillResultUI::SKILL_PLAYER_HEAL, { 88.0f,381.0f }, -200.0f);

	//カウントSE
	for (int i = 0; i < SET_PRISM_COUNT_SE_MAX; ++i)
	{
		std::string fileName = "attack_count_" + std::to_string(i) + ".wav";
		m_countSeArray[i] = AudioApp::Instance()->LoadAudio("resource/user/sound/attack_count/" + fileName, 0.5f);
	}
}

void SetPrismEffect::Init()
{
	using namespace KuroEngine;

	m_isActive = false;

	for (int enemyIdx = 0; enemyIdx < ENEMY_COUNT_MAX; ++enemyIdx)
		m_enemyDamageUI[enemyIdx].Init();

}

void SetPrismEffect::Update(std::weak_ptr<PanelManager>arg_panelManager)
{
	if (!m_isActive)return;

	for (auto& ui : m_activeUIArray)
	{
		ui->Update();
	}

	m_setPrismTimer.UpdateTimer();
	if (m_setPrismTimer.IsTimeUpOnTrigger())
	{
		arg_panelManager.lock()->JudgeSet(m_setChipIdxArray.back(), m_color);
		m_setChipIdxArray.pop_back();

		for (auto& ui : m_activeUIArray)
		{
			ui->Add(m_amountPerOneBlock, true);
		}

		//SE再生
		KuroEngine::AudioApp::Instance()->PlayWave(m_countSeArray[std::min(m_countSeIdx++, static_cast<int>(m_countSeArray.size()) - 1)]);

		if (!m_setChipIdxArray.empty())
			m_setPrismTimer.Reset();
	}

	//敵への与ダメージ演出終了したか
	if (m_setChipIdxArray.empty() && !m_activeUIArray[0]->GetIsActive())
	{
		m_isActive = false;
		m_activeUIArray.clear();

		// 設置したらアクション
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction(m_skillName, m_blockCount, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
}

void SetPrismEffect::Draw()
{
	if (!m_isActive)return;

	for (auto& ui : m_activeUIArray)
	{
		ui->Draw();
	}
}

void SetPrismEffect::Start(std::vector<KuroEngine::Vec2<int>> arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, std::string arg_skillName, std::vector<int>arg_targetEnemyIdxArray)
{
	using namespace KuroEngine;

	CommonInitOnStart(arg_setChipIdxArray, arg_color, arg_damagePerOneBlock, arg_skillName);
	m_targetEnemyIdxArray = arg_targetEnemyIdxArray;

	for (auto& enemyIdx : m_targetEnemyIdxArray)
	{
		if (enemyIdx < 0)continue;
		if (ENEMY_COUNT_MAX <= enemyIdx)continue;

		m_activeUIArray.emplace_back(&m_enemyDamageUI[enemyIdx]);
	}
}

void SetPrismEffect::Start(std::vector<KuroEngine::Vec2<int>> arg_setChipIdxArray, BlockColor arg_color, int arg_healPerOneBlock, std::string arg_skillName)
{
	CommonInitOnStart(arg_setChipIdxArray, arg_color, arg_healPerOneBlock, arg_skillName);
	m_activeUIArray = { &m_playerHealUI };
}
