#include "SkillResultUI.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"KuroEngine.h"
#include"../Effect/ParticleManager.h"
#include"../Effect/ScreenShakeManager.h"
#include"../SoundConfig.h"
#include"../RefreshRate.h"

SkillResultUI::SkillResultUI()
{
	using namespace KuroEngine;

	std::array<std::string, SKILL_NUM>fileNameArray =
	{
		"damage",
		"heal"
	};

	std::string dir = "resource/user/tex/battle_scene/";

	for (int skillIdx = 0; skillIdx < SKILL_NUM; ++skillIdx)
	{
		//スキル効果画像
		m_skillTex[skillIdx] = D3D12App::Instance()->GenerateTextureBuffer(dir + fileNameArray[skillIdx] + ".png");

		//総量数字画像
		D3D12App::Instance()->GenerateTextureBuffer(
			m_amountNumTex[skillIdx].data(), dir + fileNameArray[skillIdx] + "_number.png", 10, Vec2(10, 1));

		//履歴数字画像
		D3D12App::Instance()->GenerateTextureBuffer(
			m_historyNumTex[skillIdx].data(), dir + fileNameArray[skillIdx] + "_rireki_number.png", 11, Vec2(11, 1));
	}
}

void SkillResultUI::Init()
{
	m_isActive = false;
	m_impactShake.Init();
}

#include "../BattleManager/ExistUnits.h"
#include "../BattleManager/Enemy/Enemy.h"


void SkillResultUI::Update(std::weak_ptr<ParticleEmitter>arg_ultParticleEmitter)
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	m_impactShake.Update(1.0f);

	//退場開始までの時間
	if (m_appearTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag))
	{
		//退場中
		m_disappearTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag);
		m_nowPos.x = Math::Ease(In, Back, m_disappearTimer.GetTimeRate(), m_appearPos.x, m_disappearPosX);

		//退場完了
		if (m_disappearTimer.IsTimeUp())
		{
			//履歴削除
			m_history.clear();
			m_isActive = false;

			//ULTのパーティクルを出す
			if (m_skillType == SKILL_ENEMY_DAMAGE)
			{
				bool isBoss = false;
				for (auto& data : ExistUnits::Instance()->m_Enemys) {
					if (GetUnitPtr_nama<Enemy*>(data)->m_Data.m_Tag == EnemysData::ENEMY_TAG::BOSS) isBoss = true;
				}
				arg_ultParticleEmitter.lock()->Emit({ m_disappearPosX,m_appearPos.y }, isBoss ? m_amount * 3 : m_amount);
				//画面揺らし
				ScreenShakeManager::Instance()->Shake();

				SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE, 2);
			}
		}
	}

	//履歴のアルファ
	for (auto itr = m_history.begin(); itr != m_history.end(); ++itr)
	{
		size_t idx = std::distance(m_history.begin(), itr);
		float baseAlpha = 1.0f - static_cast<float>(idx) / m_history.size();
		itr->second = Math::Lerp(baseAlpha, 0.0f, m_appearTimer.GetTimeRate(std::min(1.0f, baseAlpha + 0.5f)));
	}
}

void SkillResultUI::Draw()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	//ダメージ数描画オフセット座標
	const Vec2<float>NUMBER_OFFSET_POS = { 0.0f,-57.0f };

	Vec2<float>shake = { 0.0f,m_impactShake.GetOffset().y };
	auto pos = m_nowPos + shake;
	DrawFunc2D::DrawRotaGraph2D(pos, {1.0f,1.0f}, 0.0f, m_skillTex[m_skillType]);
	DrawFunc2D::DrawNumber2D(m_amount, pos + NUMBER_OFFSET_POS,
		m_amountNumTex[m_skillType].data(), {1.0f,1.0f}, 1.0f, 0.0f,
		HORIZONTAL_ALIGN::CENTER, VERTICAL_ALIGN::CENTER, 2);

	//最新のダメージ履歴の描画オフセット座標
	const Vec2<float>DAMAGE_HIS_OFFSET_POS = { 40.0f,22.0f };
	//履歴間の行間
	const float DAMAGE_HIS_LINE_SPACE = 29.0f;

	auto damageHisPos = m_appearPos + shake + DAMAGE_HIS_OFFSET_POS;
	for (auto itr = m_history.begin(); itr != m_history.end(); ++itr)
	{
		DrawFunc2D::DrawNumber2D(itr->first, damageHisPos,
			m_historyNumTex[m_skillType].data(), {1.0f,1.0f}, itr->second, 0.0f, HORIZONTAL_ALIGN::RIGHT,
			VERTICAL_ALIGN::TOP, -1, 10, -1);

		//行間ずらし
		damageHisPos.y += DAMAGE_HIS_LINE_SPACE;
	}
}

void SkillResultUI::Add(int arg_damage, bool arg_drawHistory)
{
	//振動パラメータ
	const float SHAKE_TIME = 15.0f;
	const float SHAKE_SPAN = 2.0f;
	const float SHAKE_POWER_MIN = 6.0f;
	const float SHAKE_POWER_MAX = 18.0f;

	//表示時間
	const float APPEAR_TIME = 45.0f;
	m_appearTimer.Reset(APPEAR_TIME);

	//退場時間
	const float DISAPPEAR_TIME = 25.0f;
	m_disappearTimer.Reset(DISAPPEAR_TIME);

	//与ダメージ最大履歴数
	const int QUEUE_MAX = 3;

	//非表示状態
	if (!m_isActive)
	{
		m_amount = 0;
	}

	m_isActive = true;
	m_amount += arg_damage;
	m_impactShake.Shake(SHAKE_TIME, SHAKE_SPAN, SHAKE_POWER_MIN, SHAKE_POWER_MAX);
	m_nowPos = m_appearPos;

	if (arg_drawHistory)
	{
		m_history.push_front({ arg_damage,1.0f });
		if (QUEUE_MAX < static_cast<int>(m_history.size()))
		{
			m_history.pop_back();
		}
	}
}