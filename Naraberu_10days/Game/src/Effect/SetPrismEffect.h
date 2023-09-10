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
class ParticleEmitter;

//プリズムを置いたときの演出
class SetPrismEffect
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

	//プレイヤーに対する回復UI
	SkillResultUI m_playerHealUI;

	//起動中のUI
	std::vector<SkillResultUI*>m_activeUIArray;

	//ターゲットの敵
	std::vector<int>m_targetEnemyIdxArray;

	//1ブロックにつきどれくらいの効果があるか
	int m_amountPerOneBlock = 1;

	//１ブロックを置くごとのタイマー
	KuroEngine::Timer m_setPrismTimer;

	//攻撃カウント最大
	static const int SET_PRISM_COUNT_SE_MAX = 10;
	//攻撃カウントSE
	std::array<int, SET_PRISM_COUNT_SE_MAX>m_countSeArray;
	//再生するカウントSEのインデックス
	int m_countSeIdx;

	std::string m_skillName;

	//共通のスタート時の初期化
	void CommonInitOnStart(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_amountPerOneBlock, std::string arg_skillName);

public:
	SetPrismEffect();
	void Init();
	void Update(std::weak_ptr<PanelManager>arg_panelManager, std::weak_ptr<ParticleEmitter>arg_ultParticleEmitter);
	void Draw();

	/// <summary>
	/// 演出スタート（全体）
	/// </summary>
	/// <param name="arg_setChipIdxArray">設置位置インデックスの配列</param>
	/// <param name="arg_color">ブロックの色</param>
	/// <param name="arg_damagePerOneBlock">1ブロックにつき何ダメージか</param>
	/// <param name="arg_skillName">スキル名</param>
	/// <param name="arg_targetEnemyIdxArray">ターゲットの敵のインデックス配列</param>
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_damagePerOneBlock, std::string arg_skillName, std::vector<int>arg_targetEnemyIdxArray);
	
	void Start(std::vector<KuroEngine::Vec2<int>>arg_setChipIdxArray, BlockColor arg_color, int arg_healPerOneBlock, std::string arg_skillName);

	//アクティブ状態ゲッタ
	const bool& GetIsActive()const { return m_isActive; }
};