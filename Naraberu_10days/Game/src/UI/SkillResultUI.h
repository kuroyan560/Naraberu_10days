#pragma once
#include<array>
#include<memory>
#include"Common/Vec.h"
#include<vector>
#include"ForUser/Timer.h"
#include"ForUser/ImpactShake.h"
#include<list>

namespace KuroEngine
{
	class TextureBuffer;
};

class ParticleEmitter;

class SkillResultUI
{
public:
	enum SKILL
	{
		SKILL_ENEMY_DAMAGE,
		SKILL_PLAYER_HEAL,
		SKILL_ENEMY_DAMAGE_PERFECT,
		SKILL_NUM
	};

private:
	//効果画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, SKILL_NUM>m_skillTex;
	//デカ数字画像
	std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10>, SKILL_NUM>m_amountNumTex;
	//履歴数字画像（１１枚なのは「+」分）
	std::array <std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 12>, SKILL_NUM>m_historyNumTex;

	//振動
	KuroEngine::ImpactShake m_impactShake;

	//出現タイマー
	KuroEngine::Timer m_appearTimer;

	//退場タイマー
	KuroEngine::Timer m_disappearTimer;

	//アクティブ状態
	bool m_isActive = false;

	//スキル種別
	SKILL m_skillType;

	//総量
	int m_amount = 0;

	//UI初期化座標
	KuroEngine::Vec2<float>m_appearPos = { 200.0f,200.0f };
	//UI退場後のX座標
	float m_disappearPosX = 0.0f;

	KuroEngine::Timer m_expandTimer;
	float m_expand = 1.0f;

	//現在のUI表示座標
	KuroEngine::Vec2<float>m_nowPos;

	struct HistoryInfo
	{
		int m_amount = 0;
		bool m_isMul = false;
		float m_alpha = 1.0f;

		HistoryInfo(int arg_amount, bool arg_isMul)
			:m_amount(arg_amount), m_isMul(arg_isMul) {}
	};
	//与ダメージ最大履歴数
	//static const int QUEUE_MAX = 3;
	//与ダメージ履歴キュー
	std::list<HistoryInfo>m_history;

	bool m_stopDisappear = false;

	KuroEngine::Timer m_disappearDelayTimer;

	void CommonInitOnStart(float arg_appearTime, float arg_disappearDelay);
public:
	SkillResultUI();
	void Init();
	void Update(std::weak_ptr<ParticleEmitter>arg_ultParticleEmitter);
	void Draw();

	void Add(int arg_damage, bool arg_drawHistory, float arg_disappearDelay);
	void Mul(int arg_mulAmount, bool arg_drawHistory, bool arg_isPerfect, float arg_disappearDelay);

	//登場位置の設定
	void Set(SKILL arg_skill, KuroEngine::Vec2<float>arg_appearPos, float arg_disappearPosX)
	{
		m_skillType = arg_skill;
		m_appearPos = arg_appearPos;
		m_disappearPosX = arg_disappearPosX;
	}

	//アクティブ状態のゲッタ
	const bool& GetIsActive()const { return m_isActive; }

	void SetStopDisappear(bool arg_flg);
};