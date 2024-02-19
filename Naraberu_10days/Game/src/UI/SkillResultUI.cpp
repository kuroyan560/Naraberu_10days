#include "SkillResultUI.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"KuroEngine.h"
#include"../Effect/ParticleManager.h"
#include"../Effect/ScreenShakeManager.h"
#include"../SoundConfig.h"
#include"../RefreshRate.h"

void SkillResultUI::CommonInitOnStart(float arg_appearTime, float arg_disappearDelay)
{
	//�U���p�����[�^
	const float SHAKE_TIME = 15.0f;
	const float SHAKE_SPAN = 2.0f;
	const float SHAKE_POWER_MIN = 6.0f;
	const float SHAKE_POWER_MAX = 18.0f;

	//�\������
	m_appearTimer.Reset(arg_appearTime);

	//�ҋ@����
	m_disappearDelayTimer.Reset(arg_disappearDelay);

	//�ޏꎞ��
	const float DISAPPEAR_TIME = 25.0f;
	m_disappearTimer.Reset(DISAPPEAR_TIME);

	//��\�����
	if (!m_isActive)
	{
		m_amount = 0;
		if (m_skillType != SKILL_PLAYER_HEAL)m_skillType = SKILL_ENEMY_DAMAGE;
	}

	m_isActive = true;
	m_impactShake.Shake(SHAKE_TIME, SHAKE_SPAN, SHAKE_POWER_MIN, SHAKE_POWER_MAX);
	m_nowPos = m_appearPos;
	m_expand = 1.0f;
}

SkillResultUI::SkillResultUI()
{
	using namespace KuroEngine;

	std::array<std::string, SKILL_NUM>fileNameArray =
	{
		"damage",
		"heal",
		"perfect"
	};

	std::string dir = "resource/user/tex/battle_scene/";

	for (int skillIdx = 0; skillIdx < SKILL_NUM; ++skillIdx)
	{
		//�X�L�����ʉ摜
		m_skillTex[skillIdx] = D3D12App::Instance()->GenerateTextureBuffer(dir + fileNameArray[skillIdx] + ".png");

		//���ʐ����摜
		D3D12App::Instance()->GenerateTextureBuffer(
			m_amountNumTex[skillIdx].data(), dir + fileNameArray[skillIdx] + "_number.png", 10, Vec2(10, 1));

		//���𐔎��摜
		D3D12App::Instance()->GenerateTextureBuffer(
			m_historyNumTex[skillIdx].data(), dir + fileNameArray[skillIdx] + "_rireki_number.png", 12, Vec2(12, 1));
	}
}

void SkillResultUI::Init()
{
	m_isActive = false;
	m_impactShake.Init();
	m_stopDisappear = false;
}

#include "../BattleManager/ExistUnits.h"
#include "../BattleManager/Enemy/Enemy.h"


void SkillResultUI::Update(std::weak_ptr<ParticleEmitter>arg_ultParticleEmitter)
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	m_impactShake.Update(1.0f);

	//�ޏ�J�n�܂ł̎���
	if (m_appearTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag)
		&& m_disappearDelayTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag)
		&& !m_stopDisappear)
	{
		//�ޏꒆ
		m_disappearTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag);
		m_nowPos.x = Math::Ease(In, Back, m_disappearTimer.GetTimeRate(), m_appearPos.x, m_disappearPosX);

		//�ޏꊮ��
		if (m_disappearTimer.IsTimeUp())
		{
			//�����폜
			m_history.clear();
			m_isActive = false;

			//ULT�̃p�[�e�B�N�����o��
			if (m_skillType == SKILL_ENEMY_DAMAGE || m_skillType == SKILL_ENEMY_DAMAGE_PERFECT)
			{
				bool isBoss = false;
				for (auto& data : ExistUnits::Instance()->m_Enemys) {
					if (GetUnitPtr_nama<Enemy*>(data)->m_Data.m_Tag == EnemysData::ENEMY_TAG::BOSS) isBoss = true;
				}
				arg_ultParticleEmitter.lock()->Emit({ m_disappearPosX,m_appearPos.y }, isBoss ? m_amount * 3 : m_amount);
				//��ʗh�炵
				ScreenShakeManager::Instance()->Shake();

				SoundConfig::Instance()->Play(SoundConfig::SE_DAMAGE, 2);
			}
		}
	}

	//�����̃A���t�@
	for (auto itr = m_history.begin(); itr != m_history.end(); ++itr)
	{
		size_t idx = std::distance(m_history.begin(), itr);
		float baseAlpha = 1.0f - static_cast<float>(idx) / m_history.size();
		itr->m_alpha = Math::Lerp(baseAlpha, 0.0f, m_appearTimer.GetTimeRate(std::min(1.0f, baseAlpha + 0.5f)));
	}

	//if (m_skillType == SKILL_ENEMY_DAMAGE_PERFECT)
	{
		m_expandTimer.UpdateTimer(1.0f / RefreshRate::RefreshRate_Mag);
		m_expand = Math::Ease(Out, Elastic, m_expandTimer.GetTimeRate(), 5.0f, 1.0f);
	}
}

void SkillResultUI::Draw()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	//�_���[�W���`��I�t�Z�b�g���W
	const Vec2<float>NUMBER_OFFSET_POS = { 0.0f,-57.0f };

	Vec2<float>shake = { 0.0f,m_impactShake.GetOffset().y };
	auto pos = m_nowPos + shake;
	DrawFunc2D::DrawRotaGraph2D(pos, {m_expand,m_expand }, 0.0f, m_skillTex[m_skillType]);
	DrawFunc2D::DrawNumber2D(m_amount, pos + NUMBER_OFFSET_POS,
		m_amountNumTex[m_skillType].data(), { m_expand,m_expand }, 1.0f, 0.0f,
		HORIZONTAL_ALIGN::CENTER, VERTICAL_ALIGN::CENTER, 2);

	//�ŐV�̃_���[�W�����̕`��I�t�Z�b�g���W
	const Vec2<float>DAMAGE_HIS_OFFSET_POS = { 40.0f,22.0f };
	//�����Ԃ̍s��
	const float DAMAGE_HIS_LINE_SPACE = 29.0f;

	auto damageHisPos = m_appearPos + shake + DAMAGE_HIS_OFFSET_POS;
	for (auto itr = m_history.begin(); itr != m_history.end(); ++itr)
	{
		DrawFunc2D::DrawNumber2D(itr->m_amount, damageHisPos,
			m_historyNumTex[m_skillType].data(), { m_expand,m_expand }, itr->m_alpha, 0.0f, HORIZONTAL_ALIGN::RIGHT,
			VERTICAL_ALIGN::TOP, -1, itr->m_isMul ? 11 : 10);

		//�s�Ԃ��炵
		damageHisPos.y += DAMAGE_HIS_LINE_SPACE;
	}
}

void SkillResultUI::Add(int arg_damage, bool arg_drawHistory, float arg_disappearDelay)
{
	const float APPEAR_TIME = 45.0f;

	CommonInitOnStart(APPEAR_TIME, arg_disappearDelay);

	m_amount += arg_damage;
	if (arg_drawHistory)
	{
		m_history.push_front(HistoryInfo(arg_damage, false));
		if (QUEUE_MAX < static_cast<int>(m_history.size()))
		{
			m_history.pop_back();
		}
	}
}

void SkillResultUI::Mul(int arg_mulAmount, bool arg_drawHistory, bool arg_isPerfect, float arg_disappearDelay)
{
	if (m_amount == 0)return;

	const float APPEAR_TIME = 90.0f;

	CommonInitOnStart(APPEAR_TIME, arg_disappearDelay);

	m_amount *= arg_mulAmount;
	if (arg_drawHistory)
	{
		m_history.push_front(HistoryInfo(arg_mulAmount, true));
		if (QUEUE_MAX < static_cast<int>(m_history.size()))
		{
			m_history.pop_back();
		}
	}

	m_expandTimer.Reset(20.0f);
	if (arg_isPerfect && m_skillType != SKILL_PLAYER_HEAL)m_skillType = SKILL_ENEMY_DAMAGE_PERFECT;
	SoundConfig::Instance()->Play(SoundConfig::SE_PERFECT_BONUS_DAMAGE, 5);
}
