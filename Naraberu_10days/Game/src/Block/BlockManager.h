#pragma once
#include "../Block/BaseInformation.h"
#include "../Block/Block.h"
#include <memory>
#include <array>

class BlockManager
{
private:

	enum class ObjectType {
		use,//使用しているブロック
		choice1,//選択ブロック1
		choice2,//選択ブロック2
		nextChoice1,//次の選択ブロック1
		nextChoice2,//次の選択ブロック2
		size,
	};
public:
	//実際に使っているブロックの情報
	struct UseBlockType {
		int blockNum;
		BlockColor color;
		BlockAttribute attribute;
		std::shared_ptr<Block> block;
	};

	struct TutorialBlock {
		int blockNum;
		BlockColor color;
		BlockAttribute attribute;
	};

public:

	BlockManager() {};
	~BlockManager() {};

	void Initialize();

	void Update();

	void Draw();

	void Reset();

	/// <summary>
	/// ブロック選択
	/// </summary>
	void ChoiceBlock();

	/// <summary>
	/// ブロックの変更
	/// </summary>
	/// <returns></returns>
	void ChangeBlock();

	/// <summary>
	/// 中央指定
	/// </summary>
	/// <param name="_center"></param>
	void SetCenter(const KuroEngine::Vec2<int> _center) { center = _center; }

	/// <summary>
	/// 配置ブロック情報セット
	/// </summary>
	/// <param name="_nowMapchip">現在のマップチップ番号</param>
	/// <param name="_shape">形状</param>
	/// <param name="_color">色</param>
	void GetBlock(KuroEngine::Vec2<int>* _nowMapchip, std::vector<KuroEngine::Vec2<int>>* _shape,
		BlockAttribute* _attribute, BlockColor* _color) {
		*_attribute = block[int(ObjectType::use)].attribute;
		*_color = block[int(ObjectType::use)].color;
		*_shape = shape[block[int(ObjectType::use)].blockNum];
		*_nowMapchip = block[int(ObjectType::use)].block->GetMapChipNum();
	}

	/// <summary>
	/// ブロックを変更する際の処理
	/// </summary>
	/// <param name="a1">ブロック１</param>
	/// <param name="a2">ブロック２</param>
	void SetOneChangeBlock(const int a1, const int a2);

	/// <summary>
	/// パスのクールタイム演出
	/// </summary>
	void PassRecharge();

	// パス回復
	void ResetPass() { passNum = passMaxNum; }

	//チュートリアル
	void SetTutorial() { isTutorial = true; }

private:

	static const int tutorialMaxNum = 1;
	static const std::array<std::array<TutorialBlock, 10>, tutorialMaxNum>tutorialBlock;
	int tutorialNum;
	int tutorialBlockNum;
	//チュートリアル
	bool isTutorial;
	//矢印画像
	std::shared_ptr<KuroEngine::TextureBuffer> arrowTex;
	//パス画像
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> passTex;
	//数字
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,3> numTex;
	//中央
	KuroEngine::Vec2<int> center;
	//使うブロック情報
	std::array<UseBlockType, int(ObjectType::size)> block;
	//選択中のブロック
	int nowChoice;
	//パス回数
	const int passMaxNum=2;
	//パス残り回数
	int passNum;
	//パスのクールタイム
	float recharge;
	//回転
	float passEffectRota;

};

