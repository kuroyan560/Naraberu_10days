#pragma once
#include"../Block/BaseInformation.h"
#include"Common/Vec.h"
#include<vector>
#include"../UI/SkillResultUI.h"
#include<array>
#include"ForUser/Timer.h"
#include<memory>
#include<string>

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
	std::array<SkillResultUI, ENEMY_COUNT_MAX>m_enemyDamageUI;

	//ターゲットの敵
	std::vector<int>m_targetEnemyIdxArray;

	//1ブロックにつき何ダメージか
	int m_damagePerOneBlock = 1;

	//１ブロックを置くごとのタイマー
	KuroEngine::Timer m_setBlockTimer;

	//攻撃カウント最大
	static const int ATTACK_COUNT_SE_MAX = 10;
	//攻撃カウントSE
	std::array<int, ATTACK_COUNT_SE_MAX>m_countSeArray;
	//再生するカウントSEのインデックス
	int m_countSeIdx;

	std::string m_skillName;

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
	/// <param name="arg_targetEnemyIdxArray">ターゲットの敵のインデックス配列</param>
	/// <param name="arg_skillName">スキル名</param>
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, std::vector<int>arg_targetEnemyIdxArray, std::string arg_skillName);

	//アクティブ状態ゲッタ
	const bool& GetIsActive()const { return m_isActive; }
};