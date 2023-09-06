#pragma once
#include "BaseInformation.h"
#include "KuroEngineDevice.h"

class StageManager
{
public:

	StageManager(){};
	~StageManager(){};

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
	/// 塊判定
	/// </summary>
	/// <param name="_massNum">塊の数</param>
	/// <param name="_color">塊の色</param>
	bool MassProcess(std::vector<int>* _massNum, std::vector<BlockColor>* _color);

	/// <summary>
	/// 塊判定処理
	/// </summary>
	/// <param name="_massNum">消去数</param>
	/// <param name="_massMap">現在のmapchip番号</param>
	void MassBlock(int* _massNum, const KuroEngine::Vec2<int> _massMap);

	/// <summary>
	/// 塊判定
	/// </summary>
	/// <param name="lineNum">線の数</param>
	/// <param name="_color">線の色</param>
	bool LineProcess(int* _lineNum, std::vector<BlockColor>* _color);

	/// <summary>
	/// 塊判定処理
	/// </summary>
	/// <param name="_lineMap">現在のmapchip番号</param>
	/// <param name="_direction">ラインの向き false->+y/true->+x</param>
	int LineBlock(const KuroEngine::Vec2<int> _lineMap, const bool _direction);

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

private:

	//ステージの最大値
	static KuroEngine::Vec2<int> mapMax;
	//mapchip
	std::vector<std::vector<int>> mapchip;
	//塊用mapchip変数
	std::vector<std::vector<int>> massMapchip;
	//ライン用mapchip変数
	std::vector<std::vector<int>> lineMapchip;
	//ブロック画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size)> blockTex;

};

