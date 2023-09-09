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
	KuroEngine::Vec2<float>m_appearPos = { 200.0f,200.0f };

	//現在のUI表示座標
	KuroEngine::Vec2<float>m_nowPos;

	//与ダメージ履歴キュー
	std::list<std::pair<int, float>>m_history;

public:
	EnemyDamageUI();
	void Init();
	void Update();
	void Draw();

	void Add(int arg_damage, bool arg_drawHistory);

	//登場位置の設定
	void SetAppearPos(KuroEngine::Vec2<float>arg_appearPos)
	{
		m_appearPos = arg_appearPos;
	}

	//アクティブ状態のゲッタ
	const bool& GetIsActive()const { return m_isActive; }
};