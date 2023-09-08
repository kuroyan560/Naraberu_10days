#pragma once
#include<array>
#include<memory>
#include"Common/Vec.h"
#include<vector>
#include"ForUser/Timer.h"
#include"ForUser/ImpactShake.h"

namespace KuroEngine
{
	class TextureBuffer;
};

class EnemyDamageUI
{
	//「Damage」画像
	std::shared_ptr<KuroEngine::TextureBuffer>m_damageTex;
	//ダメージ数字画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10>m_damageNumTex;
	//ダメージ履歴数字画像（１１枚なのは「+」分）
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11>m_damageHisNumTex;

	//振動
	KuroEngine::ImpactShake m_impactShake;

	//出現タイマー
	KuroEngine::Timer m_appearTimer;

	//退場タイマー
	KuroEngine::Timer m_disappearTimer;

	bool m_isActive = false;
	int m_damageAmount = 0;

	//UI初期化座標
	KuroEngine::Vec2<float>m_appearPos;

	//現在のUI表示座標
	KuroEngine::Vec2<float>m_nowPos;

public:
	EnemyDamageUI();
	void Init(KuroEngine::Vec2<float>arg_appearPos);
	void Update();
	void Draw();

	void Add(int arg_damage);
};