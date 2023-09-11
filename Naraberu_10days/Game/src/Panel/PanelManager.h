#pragma once
#include "../Block/BaseInformation.h"
#include "KuroEngineDevice.h"

class SetPrismEffect;

class PanelManager
{
public:

	struct BonusData {
		std::vector<KuroEngine::Vec2<int>> pos;
		BlockColor color;
		bool mass;
	};

	enum class Bonus {
		non,
		count,
		add,
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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
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
	/// ボーナスの算出
	/// </summary>
	void BonusCount();

	/// <summary>
	/// ボーナス演出
	/// </summary>
	void BonusDirection();

	/// <summary>
	/// お邪魔セット
	/// </summary>
	/// <param name="_mapchip">セットするマップチップ番号</param>
	void SetObstacle(const KuroEngine::Vec2<int> _mapchip) {
		mapchip[_mapchip.y][_mapchip.x] = 4;
	}

	/// <summary>
	/// 中央出力
	/// </summary>
	/// <returns></returns>
	KuroEngine::Vec2<int> GetCenter() { return { int(mapchip.size()) / 2,int(mapchip[1].size()) / 2 }; }

	static KuroEngine::Vec2<int> GetMapMax(){return mapMax;}

	std::vector<std::vector<int>>* GetMapChipPtr() { return &mapchip; }

	void SetBouns() { isBonusDirection = Bonus::count; }

private:
	
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
};
