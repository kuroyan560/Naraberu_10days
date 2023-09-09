#pragma once
#include"../Block/BaseInformation.h"
#include"Common/Vec.h"
#include<vector>
#include"../UI/EnemyDamageUI.h"
#include<array>
#include"ForUser/Timer.h"
#include<memory>

class PanelManager;

//攻撃のブロックを置いたときの演出
class PlayerAttackEffect
{
	bool m_isActive = false;

	//設置位置インデックスの配列
	std::vector<KuroEngine::Vec2<int>>m_setChipIdxArray;

	//設置数
	int m_blockCount = 0;

	//ブロックの色
	BlockColor m_color;

	//敵に対する与ダメージUI
	static const int ENEMY_COUNT_MAX = 3;
	std::array<EnemyDamageUI, ENEMY_COUNT_MAX>m_enemyDamageUI;

	//ターゲットの敵
	static const int ALL_ENEMY_ATTACK = -1;
	int m_targetEnemyIdx = ALL_ENEMY_ATTACK;

	//1ブロックにつき何ダメージか
	int m_damagePerOneBlock = 1;

	//１ブロックを置くごとのタイマー
	KuroEngine::Timer m_setBlockTimer;

public:
	PlayerAttackEffect();
	void Init();
	void Update(std::weak_ptr<PanelManager>arg_panelManager);
	void Draw();

	/// <summary>
	/// 演出スタート（全体）
	/// </summary>
	/// <param name="arg_setChipIdxArray">設置位置インデックスの配列</param>
	/// <param name="arg_color">ブロックの色</param>
	/// <param name="arg_damagePerOneBlock">1ブロックにつき何ダメージか</param>
	/// <param name="arg_targetEnemy">特定の敵に対する攻撃ならその対象は誰か（デフォルトは全体攻撃扱い）</param>
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, int arg_targetEnemyIdx = ALL_ENEMY_ATTACK);

	//アクティブ状態ゲッタ
	const bool& GetIsActive()const { return m_isActive; }
};