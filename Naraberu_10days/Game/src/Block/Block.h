#pragma once
#include "KuroEngineDevice.h"
#include "../Block/BaseInformation.h"

class Block
{
public:

	Block(bool _isUse = false);
	~Block(){};

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_texNum">画像番号</param>
	/// <param name="_color">色</param>
	void Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const BlockAttribute _attribute, const BlockColor _color);

	/// <summary>
	/// 座標有の描画
	/// </summary>
	/// <param name="_texNum">画像番号</param>
	/// <param name="_color">色</param>
	/// <param name="_pos">座標</param>
	void Draw(const std::vector<KuroEngine::Vec2<int>> _shape, const KuroEngine::Vec2<float> shape_dist, const BlockAttribute _attribute, const BlockColor _color, const KuroEngine::Vec2<float>& _pos);

	/// <summary>
	/// 初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 使用するブロックが変わる場合の処理
	/// </summary>
	/// <param name="_mapchipNum">真ん中のmapchip番号</param>
	/// <param name="_shape">形状</param>
	void ChangeBlock(const KuroEngine::Vec2<int> _mapchipNum, const std::vector<KuroEngine::Vec2<int>> _shape);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="_color">色</param>
	void BlockOneDraw(const KuroEngine::Vec2<float> pos, const BlockColor _color);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_shape">形状</param>
	/// <param name="pos">座標</param>
	/// <param name="_color">色</param>
	void BlockOneDraw(const KuroEngine::Vec2<int> _shape,
		const KuroEngine::Vec2<float> pos, const BlockColor _color);

	/// <summary>
	/// action描画
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_attribute">アクション</param>
	void ActionDraw(const KuroEngine::Vec2<float> _pos,const BlockAttribute _attribute);

	KuroEngine::Vec2<int> GetMapChipNum() { return pos; }

private:

	//ブロック画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size) - 2> blockTex;
	//射線ブロック
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockColor::size) - 1> lineTex;
	//アクション画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, int(BlockAttribute::size)> actionTex;
	//座標(mapchip)
	KuroEngine::Vec2<int> pos;
	//移動可能か
	bool isMove;
	//形状最小
	KuroEngine::Vec2<int> shapeMin;
	//形状最大
	KuroEngine::Vec2<int> shapeMax;
};

