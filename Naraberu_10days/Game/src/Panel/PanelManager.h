#pragma once
#include "../Block/BaseInformation.h"
#include "KuroEngineDevice.h"
#include<vector>
#include<memory>

class SetPrismEffect;
class SkillResultUI;
class PerfectBonusEffect;

class PanelManager
{
public:

	enum class BonusKind {
		color,
		line,
		gold,
		size,
	};

	struct BonusData {
		BonusKind bonusKind;
		std::vector<KuroEngine::Vec2<int>> pos;
		BlockColor color;
		bool mass;
	};

	enum class Bonus {
		non,
		count,
		add,
		perfect,
	};

	enum class PanelSize {
		_8x8,
		_10x10
	};

	PanelManager(){};
	~PanelManager(){};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void BonusDraw();
	
	/// <summary>
	/// 
	/// </summary>
	void Reset();

	/// <summary>
	/// セット処理
	/// </summary>
	/// <param name="_nowMapchip">セットするオブジェクトのmapchip</param>
	/// <param name="_shape">セットするオブジェクトの形状</param>
	/// <param name="_attribute">アクション</param>
	/// <param name="_color">色</param>
	/// <returns></returns>
	bool JudgeSet(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
		const BlockAttribute _attribute, BlockColor _color);

	/// <summary>
	/// 設置　by ハシモト
	/// </summary>
	/// <param name="_setChipIdx">はめるインデックス</param>
	/// <param name="_color">ブロックの色</param>
	/// <returns>設置出来たか</returns>
	bool JudgeSet(KuroEngine::Vec2<int> _setChipIdx, BlockColor _color);

	/// <summary>
	/// Judgeして演出スタート（設置は演出クラスに任せる）by ハシモト
	/// </summary>
	/// <param name="_nowMapchip">セットするオブジェクトのmapchip</param>
	/// <param name="_shape">セットするオブジェクトの形状</param>
	/// <param name="_attribute">アクション</param>
	/// <param name="_color">色</param>
	/// <param name="_playerAttackEffect">演出クラス</param>
	/// <returns>演出スタートしたか</returns>
	bool JudgeWithEffect(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
		const BlockAttribute _attribute, BlockColor _color, std::weak_ptr<SetPrismEffect>_playerAttackEffect);

	/// <summary>
	/// 塊判定
	/// </summary>
	void MassProcess();

	/// <summary>
	/// 塊判定処理
	/// </summary>
	/// <param name="_number">塊番号</param>
	/// <param name="_massNum">消去数</param>
	/// <param name="_massMap">現在のmapchip番号</param>
	void MassBlock(const int _number, int* _massNum, const KuroEngine::Vec2<int> _massMap);

	/// <summary>
	/// 塊判定
	/// </summary>
	void LineProcess();

	/// <summary>
	/// 塊判定処理
	/// </summary>
	/// <param name="_number">塊番号</param>
	/// <param name="_lineMap">現在のmapchip番号</param>
	/// <param name="_direction">ラインの向き false->+y/true->+x</param>
	bool LineBlock(int _number, const KuroEngine::Vec2<int> _lineMap, const bool _direction);

	/// <summary>
	/// 金判定
	/// </summary>
	int GoldProcess();

	/// <summary>
	/// ボーナスの算出
	/// </summary>
	void BonusCount();

	/// <summary>
	/// ボーナス演出
	/// </summary>
	void BonusDirection(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect);

	/// <summary>
	/// パーフェクト演出
	/// </summary>
	void PerfectBonus(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect);

	/// <summary>
	/// 一つのみの空きを判定
	/// </summary>
	void OneProcess();

	/// <summary>
	/// 探索
	/// </summary>
	/// <param name="_massNum">個数</param>
	/// <param name="_mapchip">番号</param>
	void OneBlock(int* _massNum, const KuroEngine::Vec2<int> _mapchip);

	/// <summary>
	/// ブロックセット
	/// </summary>
	/// <param name="_mapchip">セットするマップチップ番号</param>
	void SetBlock(const KuroEngine::Vec2<int> _mapchip,int _color) {
		mapchip[_mapchip.y][_mapchip.x] = _color;
	}

	/// <summary>
	/// お邪魔セット
	/// </summary>
	/// <param name="_mapchip">セットするマップチップ番号</param>
	void SetObstacle(const KuroEngine::Vec2<int> _mapchip) {
		mapchip[_mapchip.y][_mapchip.x] = 4;
	}


	/// <summary>
	/// 指定したマップチップの描画中心座標を求める
	/// </summary>
	/// <param name="_mapchip">中心座標を求めたいマップチップ</param>
	/// <returns>指定したマップチップの描画中心座標</returns>
	KuroEngine::Vec2<float> GetChipCenterPos(const KuroEngine::Vec2<int>_mapchip);

	/// <summary>
	/// １つだけの空きのマップチップ配列ゲッタ
	/// </summary>
	/// <returns>１つだけの空きのマップチップ配列</returns>
	const std::vector<KuroEngine::Vec2<int>>& GetOneSpacePosArray() { return onePos; }

	/// <summary>
	/// 金セット
	/// </summary>
	void SetGold();

	/// <summary>
	/// 中央出力
	/// </summary>
	/// <returns></returns>
	KuroEngine::Vec2<int> GetCenter() { return { int(mapchip.size()) / 2,int(mapchip[1].size()) / 2 }; }

	static KuroEngine::Vec2<int> GetMapMax(){return mapMax;}

	std::vector<std::vector<int>>* GetMapChipPtr() { return &mapchip; }

	void SetBouns() { isBonusDirection = Bonus::count; }

	bool IsPerfect()const;

	/// <summary>
	/// 指定したチップインデックスにブロックを置けるか
	/// </summary>
	/// <param name="_chip">マップチップインデックス</param>
	/// <returns>ブロックを置けるか</returns>
	bool CanSetBlock(KuroEngine::Vec2<int>_chip)const
	{
		return mapchip[_chip.y][_chip.x] == int(BlockColor::yuka);
	}

private:
	
	//panelの大きさ
	PanelSize panelSize;
	//block一つの大きさ
	float blockSize;
	//ボーナス種類文字画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BonusKind::size)> bonusKindTex;
	//ボーナス文字画像
	std::shared_ptr<KuroEngine::TextureBuffer> bonusTex;
	//ボーナス!!!画像
	std::shared_ptr<KuroEngine::TextureBuffer>bonusMarkTex;
	//ボーナス数字画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,10> bonusNumberTex;
	//ステージの最大値
	static KuroEngine::Vec2<int> mapMax;
	//mapchip
	std::vector<std::vector<int>> mapchip;
	//塊用mapchip変数
	std::vector<std::vector<int>> massMapchip;
	//一つのみの判定用
	std::vector<std::vector<int>> oneMapchip;
	//塊用mapchip変数
	std::vector<BonusData> bonusData;
	//ブロック画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size)> blockTex;
	//ボーナスフラグ
	Bonus isBonusDirection;
	//ボーナス処理タイマー
	float bonusTimer;
	//現在のボーナス処理を行っている番号
	int nowBonusNum;
	//ボーナス変更角度
	float bonusAngle;
	//ボーナス変更大きさ
	float bonusEaseScale;
	//ボーナス変更アルファ
	float bonusAlpha;
	//ボーナス加算時の動き
	std::vector<KuroEngine::Vec2<int>> bonusPos;
	//一つだけの空き格納
	std::vector<KuroEngine::Vec2<int>> onePos;
	//ボーナスの合計
	int totalBounsNum;
public:
	int GetTotalBonus() { return totalBounsNum; }
};
