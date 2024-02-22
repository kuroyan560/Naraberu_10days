#include "PanelManager.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"
#include "../RefreshRate.h"
#include"../Effect/SetPrismEffect.h"
#include "../Effect/ScreenShakeManager.h"
#include"../SoundConfig.h"
#include"../Effect/PerfectBonusEffect.h"

KuroEngine::Vec2<int> PanelManager::mapMax;

KuroEngine::Vec2<int> center(std::vector<KuroEngine::Vec2<int>> _shape)
{
	KuroEngine::Vec2<int> shapeMax = _shape[0];
	KuroEngine::Vec2<int> shapeMin = _shape[0];
	for (auto& i : _shape) {
		std::array<int, 4> moveHit = {
			int((i.x < shapeMin.x)),
			int((i.y < shapeMin.y)),
			int((i.x > shapeMax.x)),
			int((i.y > shapeMax.y))
		};

		shapeMin.x = moveHit[0] * i.x + (1 - moveHit[0]) * shapeMin.x;
		shapeMin.y = moveHit[1] * i.y + (1 - moveHit[1]) * shapeMin.y;
		shapeMax.x = moveHit[2] * i.x + (1 - moveHit[2]) * shapeMax.x;
		shapeMax.y = moveHit[3] * i.y + (1 - moveHit[3]) * shapeMax.y;
	}

	return { ((shapeMax.x + shapeMin.x) / 2),((shapeMax.y + shapeMin.y) / 2) };
}

void PanelManager::Initialize()
{
	//画像読み込み
	std::string TexBonusDir = "resource/user/tex/battle_scene/bonus/";
	bonusTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexBonusDir + "bonus.png");
	bonusMarkTex = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexBonusDir + "bonus_mark.png");
	KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(bonusNumberTex.data(), TexBonusDir + "bonus_number.png", 10, { 10, 1 });

	bonusKindTex[int(BonusKind::color)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexBonusDir + "color.png");
	bonusKindTex[int(BonusKind::line)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexBonusDir + "line.png");
	bonusKindTex[int(BonusKind::gold)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexBonusDir + "gold.png");

	std::string TexDir = "resource/user/tex/block/";
	blockTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_pink.png");
	blockTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_blue.png");
	blockTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_yellow.png");
	blockTex[int(BlockColor::yuka)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "yuka.png");
	blockTex[int(BlockColor::obstacle)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "oja.png");
	blockTex[int(BlockColor::eizoku_obstacle)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "eizoku_oja.png");
	blockTex[int(BlockColor::gold)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_gold.png");

	//パネルの大きさ	
	char stageName = ExistUnits::Instance()->m_StageName.back();

	if (stageName == *std::string("1").c_str() || stageName == *std::string("2").c_str()) {
		panelSize = PanelSize::_8x8;
		ExistUnits::Instance()->m_mapSize = 8;
		blockSize = blockSize8x8;
	} else {
		panelSize = PanelSize::_10x10;
		ExistUnits::Instance()->m_mapSize = 10;
		blockSize = blockSize10x10;
	}

	//配置場所の範囲決定
	if (panelSize == PanelSize::_8x8) {
		mapchip.resize(8);
		for (auto& i : mapchip) {
			for (int range = 0; range < 8; range++) {
				i.emplace_back(int(BlockColor::yuka));
			}
		}
	} else {
		mapchip.resize(10);
		for (auto& i : mapchip) {
			for (int range = 0; range < 10; range++) {
				i.emplace_back(int(BlockColor::yuka));
			}
		}
	}

	massMapchip = mapchip;
	for (auto& y : massMapchip) {
		for (auto& x : y) {
			x = 0;
		}
	}
	oneMapchip = massMapchip;

	mapMax = { int(mapchip.size()), int(mapchip[0].size()) };

	totalBounsNum = 0;
}

void PanelManager::Update(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect)
{
	if (isBonusDirection == Bonus::count) {
		BonusCount();
	}
	else if (isBonusDirection == Bonus::add) {
		BonusDirection(arg_enemyDamageUI,arg_perfectBonusEffect);
	}
	else if (isBonusDirection == Bonus::perfect) {
		PerfectBonus(arg_enemyDamageUI, arg_perfectBonusEffect);
	}

	//ボーナス表示処理
	for (auto& i : bonusData) {
		if (!i.isAlive) { continue; }
		i.timer++;
		//alpha更新
		if (i.isAlpha==0) {
			/*i.alpha = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
				i.timer, 10.0f, 0.0f, 1.0f);*/
			i.alpha = 1.0f;
			if (i.timer >= 90.0f) {
				i.isAlpha = 1;
			}
		} else if (i.isAlpha == 2) {
			i.alpha = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
				i.timer, 60.0f, 1.0f, 0.0f);

			if (i.timer >= 60.0f) {
				i.isAlive = false;
				i.isAlpha = 1;
			}
		}

		//位置
		if (i.isUp == true) {
			i.upY = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Elastic,
				i.timer, 10.0f, -120.0f, 0.0f);
			if (i.timer >= 20.0f) {
				i.isUp = false;
			}
		}

		//消える処理に移行
		if (i.timer >= 30.0f) {
			if (i.isAlpha == 1) {
				i.isAlpha = 2;
				i.timer = 0;
			}
		}
	}

	OneProcess();
}

void PanelManager::Draw()
{
	//パネル
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			KuroEngine::Vec2<float> pos = { x * blockSize + difference.x ,y * blockSize + difference.y };

			KuroEngine::Vec2<float> pos1 = pos;
			pos1.x += blockSize;
			pos1.y += blockSize;

			pos += ScreenShakeManager::Instance()->GetOffset();
			pos1 += ScreenShakeManager::Instance()->GetOffset();

			if (mapchip[y][x] == int(BlockColor::red)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::red)]);
			} else if (mapchip[y][x] == int(BlockColor::blue)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::blue)]);
			} else if (mapchip[y][x] == int(BlockColor::yellow)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::yellow)]);
			} else if (mapchip[y][x] == int(BlockColor::yuka)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::yuka)]);
			}else if (mapchip[y][x] == int(BlockColor::obstacle)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::obstacle)]);
			} else if (mapchip[y][x] == int(BlockColor::eizoku_obstacle)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::eizoku_obstacle)]);
			} else if (mapchip[y][x] == int(BlockColor::gold)) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::gold)]);
			}
		}
	}
}

void PanelManager::BonusDraw()
{
	//ボーナスパネル
	if (isBonusDirection == Bonus::add) {
		//ボーナス数表示
		for (int i = 0; i<int(bonusData.size()); i++) {
			if (i > nowBonusNum) { continue; }
			KuroEngine::Vec2<float> pos = { bonusPos[i].x * blockSize + difference.x,bonusPos[i].y * blockSize + difference.y };
			KuroEngine::Vec2<float> upPos = { 0.0f,bonusData[i].upY };

			//ボーナス文字
			if (bonusData[i].bonusKind == BonusKind::color) {
				const KuroEngine::Vec2 bonusSize = { 91.0f,26.0f };
				const KuroEngine::Vec2 distPos = { 0.0f,-25.0f };
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(
					pos + distPos + upPos, pos + bonusSize + distPos + upPos, bonusKindTex[int(BonusKind::color)], bonusData[i].alpha);
			} else if (bonusData[i].bonusKind == BonusKind::line) {
				const KuroEngine::Vec2 bonusSize = { 72.0f,26.0f };
				const KuroEngine::Vec2 distPos = { 0.0f,-25.0f };
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(
					pos + distPos + upPos, pos + bonusSize + distPos + upPos, bonusKindTex[int(BonusKind::line)], bonusData[i].alpha);
			}if (bonusData[i].bonusKind == BonusKind::gold) {
				const KuroEngine::Vec2 bonusSize = { 72.0f,26.0f };
				const KuroEngine::Vec2 distPos = { 0.0f,-25.0f };
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(
					pos + distPos + upPos, pos + bonusSize + distPos + upPos, bonusKindTex[int(BonusKind::gold)], bonusData[i].alpha);
			}

			//ボーナス文字
			const KuroEngine::Vec2 bonusSize = { 99.0f,20.0f };
			KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos + upPos, pos + bonusSize + upPos, bonusTex, bonusData[i].alpha);

			const KuroEngine::Vec2 numSize = { 18.0f,20.0f };
			KuroEngine::DrawFunc2D::DrawNumber2D(i + 1, { pos.x + numSize.x + 20.0f ,pos.y + numSize.y + upPos.y },
				bonusNumberTex.data(), { 1.0f,1.0f }, bonusData[i].alpha);
		}

		for (int i = 0; i<int(bonusData.size()); i++) {
			//描画するボーナスで無いなら抜ける
			if (i != nowBonusNum) { continue; }

			for (auto& itr : bonusData[i].pos) {
				KuroEngine::Vec2<float> pos = {
				itr.x * blockSize + difference.x + blockSize / 2.0f ,itr.y * blockSize + difference.y + blockSize / 2.0f };

				KuroEngine::DrawFunc2D::DrawRotaGraph2D(pos, { bonusEaseScale,bonusEaseScale }, bonusAngle * (3.14f / 180.0f),
					blockTex[int(bonusData[i].color)], bonusAlpha, { 0.5f,0.5f }, KuroEngine::AlphaBlendMode::AlphaBlendMode_Add);
			}
		}
	}
}

void PanelManager::Reset()
{
	for (auto& y : mapchip) {
		for (auto& x : y) {
			if (x == int(BlockColor::eizoku_obstacle)) { continue; }
			x = int(BlockColor::yuka);
			//x = int(BlockColor::red);
		}
	}
}

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"

bool PanelManager::JudgeSet(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
	const BlockAttribute _attribute, BlockColor color)
{
	for (auto& i : _shape) {
		//ステージとの判定 ダメならfalse
		if (mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] != int(BlockColor::yuka)) {
			return false;
		}
	}

	//はめられるので色変え
	int Count = 0;
	for (auto& i : _shape) {
		mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] = int(color);
		Count++;
	}

	// 設置したらアクション
	if (_attribute == BlockAttribute::attack1) {
		// 弱攻撃
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", Count, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	else if (_attribute == BlockAttribute::attack2) {
		// 強攻撃
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_02", Count, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	else if (_attribute == BlockAttribute::recovery) {
		// 回復
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Heal_01", Count, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	return true;
}

bool PanelManager::JudgeSet(KuroEngine::Vec2<int> _setChipIdx, BlockColor _color)
{
	if (mapchip[_setChipIdx.y][_setChipIdx.x] != int(BlockColor::yuka))return false;

	mapchip[_setChipIdx.y][_setChipIdx.x] = int(_color);

	return true;
}

bool PanelManager::JudgeWithEffect(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape, const BlockAttribute _attribute, BlockColor _color, std::weak_ptr<SetPrismEffect>_playerAttackEffect)
{
	//まだ演出が終わってない
	if (_playerAttackEffect.lock()->GetIsActive())return false;

	//はめるインデックスの配列構成
	std::vector<KuroEngine::Vec2<int>>setChipIdxArray;
	for (auto& i : _shape) {
		setChipIdxArray.emplace_back(_nowMapchip + i);
	}

	for (auto& i : setChipIdxArray) {
		//ステージとの判定 ダメならfalse
		if (mapchip[i.y][i.x] != int(BlockColor::yuka)) {

			SoundConfig::Instance()->Play(SoundConfig::SE_CANNOT_SELECT);

			return false;
		}
	}

	// 設置したらアクション
	if (_attribute == BlockAttribute::attack1) {
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 2, "Attack_01", { ExistUnits::Instance()->m_NowTarget });
	}
	else if (_attribute == BlockAttribute::attack2) {
		// 強攻撃
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 1, "Attack_02", ExistUnits::Instance()->GetAliveEnemyIndex());
	}
	else if (_attribute == BlockAttribute::recovery) {
		// 回復
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 1, "Heal_01");
	}
	return true;
}

void PanelManager::MassProcess()
{
	//削除管理用マップ初期化
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			massMapchip[y][x] = 0;
		}
	}
	
	//塊箇所確認
	int count = 0;
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			//確認済みなら次に行く
			if (massMapchip[y][x] != 0 || mapchip[y][x] >= int(BlockColor::yuka)) { continue; }
			//塊確認
			int massNum = 0;
			bonusData.emplace_back();
			MassBlock(count, &massNum, { x,y });
			//塊個数5個から保存
			if (massNum < 5) {
				const int size = int(bonusData.size()) - 1;
				bonusData.resize(size);
				continue;
			}
			bonusData[count].bonusKind = BonusKind::color;
			bonusData[count].color = BlockColor(mapchip[y][x]);
			bonusData[count].upY = -10.0f;
			bonusData[count].alpha = 0.0f;
			bonusData[count].mass = true;
			//座標記録
			bonusPos.emplace_back(center(bonusData[count].pos));
			//追加のボーナス+2まで
			if (massNum > 8) {
				bonusData.emplace_back(bonusData[count]);
				bonusPos.emplace_back(center(bonusData[count].pos));
				count++;
			}
			if (massNum > 11) {
				bonusData.emplace_back(bonusData[count]);
				bonusPos.emplace_back(center(bonusData[count].pos));
				count++;
			}
			if (massNum > 14) {
				bonusData.emplace_back(bonusData[count]);
				bonusPos.emplace_back(center(bonusData[count].pos));
				count++;
			}

			//次
			count++;
		}
	}
}

void PanelManager::MassBlock(const int _number, int* _massNum, const KuroEngine::Vec2<int> _massMap)
{
	//削除個数カウント
	*_massNum += 1;
	massMapchip[_massMap.y][_massMap.x] = 1;

	//下
	if (_massMap.y + 1 < mapMax.y) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y + 1][_massMap.x] &&
			massMapchip[_massMap.y + 1][_massMap.x] != 1) {
			MassBlock(_number, _massNum, { _massMap.x,_massMap.y + 1 });
		}
	}
	//上
	if (_massMap.y - 1 >= 0) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y - 1][_massMap.x] &&
			massMapchip[_massMap.y - 1][_massMap.x] != 1) {
			MassBlock(_number, _massNum, { _massMap.x,_massMap.y - 1 });
		}
	}
	//右
	if (_massMap.x + 1 < mapMax.x) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x + 1] &&
			massMapchip[_massMap.y][_massMap.x + 1] != 1) {
			MassBlock(_number, _massNum, { _massMap.x + 1,_massMap.y });
		}
	}
	//左
	if (_massMap.x - 1 >= 0) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x - 1] &&
			massMapchip[_massMap.y][_massMap.x - 1] != 1) {
			MassBlock(_number, _massNum, { _massMap.x - 1,_massMap.y });
		}
	}

	//削除場所記録
	if (*_massNum >= 5) {
		KuroEngine::Vec2<int> data = { _massMap.x,_massMap.y };
		bonusData[_number].pos.emplace_back(data);
	}
}

void PanelManager::LineProcess()
{
	//塊箇所確認
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			if (y == 0) {
				int size = int(bonusData.size());
				if (!LineBlock(size, { x,y }, false)) { continue; }
			} else if (x == 0) {
				int size = int(bonusData.size());
				if (!LineBlock(size, { x,y }, true)) { continue; }
			}
		}
	}
}

bool PanelManager::LineBlock(int _number, const KuroEngine::Vec2<int> _lineMap, const bool _direction)
{
	//下
	if (!_direction) {
		for (int i = 0; i < mapMax.y; i++) {
			if (mapchip[i][_lineMap.x] == int(BlockColor::yuka)) { return false; }
		}

		bonusData.emplace_back();

		for (int i = 0; i < mapMax.y; i++) {
			KuroEngine::Vec2<int> data = { _lineMap.x, i };
			bonusData[_number].pos.emplace_back(data);
		}
	}
	//右
	else {
		for (int i = 0; i < mapMax.x; i++) {
			if (mapchip[_lineMap.y][i] == int(BlockColor::yuka)) { return false; }
		}

		bonusData.emplace_back();

		for (int i = 0; i < mapMax.y; i++) {
			KuroEngine::Vec2<int> data = { i, _lineMap.y };
			bonusData[_number].pos.emplace_back(data);
		}
	}
	//座標記録
	bonusPos.emplace_back(center(bonusData[_number].pos));

	bonusData[_number].bonusKind = BonusKind::line;
	bonusData[_number].color = BlockColor(mapchip[_lineMap.y][_lineMap.x]);
	bonusData[_number].upY = -10.0f;
	bonusData[_number].alpha = 0.0f;
	bonusData[_number].mass = false;

	return true;
}

int PanelManager::GoldProcess()
{
	int num = 0;
	//塊箇所確認
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			if (mapchip[y][x] != int(BlockColor::gold)) { continue; }
			//ボーナス加算
			num += 1;
			BonusData add;
			add.bonusKind = BonusKind::gold;
			add.pos.emplace_back(KuroEngine::Vec2<int>(x, y));
			add.color = BlockColor(mapchip[y][x]);
			add.mass = false;
			bonusData.emplace_back(add);

			bonusPos.emplace_back(KuroEngine::Vec2<int>(x, y));
		}
	}

	return num;
}

void PanelManager::BonusCount()
{
	bonusData.clear();
	bonusPos.clear();

	//塊判定
	MassProcess();
	//ライン判定
	LineProcess();
	//金判定
	int gold = GoldProcess();

	ExistUnits::Instance()->SetBonusCount(int(bonusData.size()));

	totalBounsNum += int(bonusData.size());
	nowBonusNum = 0;
	bonusTimer = 0;
	isBonusDirection = Bonus::add;

	bonusData[nowBonusNum].isAlive = true;
	bonusData[nowBonusNum].isUp = true;
	bonusData[nowBonusNum].isAlpha = 0;
	bonusData[nowBonusNum].timer = 0;
	nowBonusNum++;
}

void PanelManager::BonusDirection(std::vector<std::weak_ptr<SkillResultUI>>arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect>arg_perfectBonusEffect)
{
	if (bonusData.empty())
	{
		isBonusDirection = Bonus::non;
		ExistUnits::Instance()->m_IsEndBonusCount = true;
		Reset();
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Bonus_01", 0, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
		return;
	}

	float maxTimer = 25.0f * RefreshRate::RefreshRate_Mag;

	if (bonusTimer == 0) {
		SoundConfig::Instance()->Play(SoundConfig::SE_BONUS_ATTACK_COUNT, -1, -1, nowBonusNum == 1);

	}

	bonusEaseScale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Back,
		bonusTimer, maxTimer, 1.2f, 1.0f);
	bonusAngle = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::Out, KuroEngine::EASING_TYPE::Elastic,
		bonusTimer, maxTimer, 0, 90.0f);
	bonusAlpha = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Circ,
		bonusTimer, maxTimer, 0.8f, 0.1f);

	//時間になったら次に行く
	bonusTimer++;
	if (bonusTimer > maxTimer) {
		if (int(bonusData.size()) <= nowBonusNum) {

			if (IsPerfect())
			{
				arg_perfectBonusEffect.lock()->Start(arg_enemyDamageUI);
				isBonusDirection = Bonus::perfect;
			}
			else if (!arg_enemyDamageUI[0].lock()->GetIsActive())
			{
				isBonusDirection = Bonus::non;
				ExistUnits::Instance()->m_IsEndBonusCount = true;
				Reset();

				// ボーナスアタック
				PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Bonus_01", ExistUnits::Instance()->m_NowBonusCount, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
				return;
			}
		}
		else
		{
			bonusData[nowBonusNum].isAlive = true;
			bonusData[nowBonusNum].isUp = true;
			bonusData[nowBonusNum].isAlpha = 0;
			bonusData[nowBonusNum].timer = 0;

			nowBonusNum++;
			bonusTimer = 0;

			for (auto& ui : arg_enemyDamageUI)
			{
				ui.lock()->Add(1, true, 60.0f);
			}
		}
	}
}

void PanelManager::PerfectBonus(std::vector<std::weak_ptr<SkillResultUI>> arg_enemyDamageUI, std::weak_ptr<PerfectBonusEffect> arg_perfectBonusEffect)
{
	if (!arg_enemyDamageUI[0].lock()->GetIsActive())
	{
		isBonusDirection = Bonus::non;
		ExistUnits::Instance()->m_IsEndBonusCount = true;
		Reset();

		// ボーナスアタック
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Bonus_01", ExistUnits::Instance()->m_NowBonusCount * 3, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
		return;
	}
}

void PanelManager::OneProcess()
{
	//削除管理用マップ初期化
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			oneMapchip[y][x] = 0;
		}
	}

	onePos.clear();

	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			//確認済みなら次に行く
			if (oneMapchip[y][x] != 0 || mapchip[y][x] != int(BlockColor::yuka)) { continue; }
			//塊確認
			int massNum = 0;
			OneBlock(&massNum, { x,y });
			//塊個数0個以外なら次に行く
			if (massNum != 1) { continue; }
			KuroEngine::Vec2<int> inPos = { x,y };
			onePos.emplace_back(inPos);
		}
	}
}

void PanelManager::OneBlock(int* _massNum, KuroEngine::Vec2<int> _mapchip)
{
	//削除個数カウント
	*_massNum += 1;
	oneMapchip[_mapchip.y][_mapchip.x] = 1;

	//下
	if (_mapchip.y + 1 < mapMax.y) {
		if (mapchip[_mapchip.y][_mapchip.x] == mapchip[_mapchip.y + 1][_mapchip.x] &&
			oneMapchip[_mapchip.y + 1][_mapchip.x] != 1) {
			OneBlock(_massNum, { _mapchip.x,_mapchip.y + 1 });
		}
	}
	//上
	if (_mapchip.y - 1 >= 0) {
		if (mapchip[_mapchip.y][_mapchip.x] == mapchip[_mapchip.y - 1][_mapchip.x] &&
			oneMapchip[_mapchip.y - 1][_mapchip.x] != 1) {
			OneBlock(_massNum, { _mapchip.x,_mapchip.y - 1 });
		}
	}
	//右
	if (_mapchip.x + 1 < mapMax.x) {
		if (mapchip[_mapchip.y][_mapchip.x] == mapchip[_mapchip.y][_mapchip.x + 1] &&
			oneMapchip[_mapchip.y][_mapchip.x + 1] != 1) {
			OneBlock(_massNum, { _mapchip.x + 1 ,_mapchip.y });
		}
	}
	//左
	if (_mapchip.x - 1 >= 0) {
		if (mapchip[_mapchip.y][_mapchip.x] == mapchip[_mapchip.y][_mapchip.x - 1] &&
			oneMapchip[_mapchip.y][_mapchip.x - 1] != 1) {
			OneBlock(_massNum, { _mapchip.x - 1,_mapchip.y });
		}
	}
}

KuroEngine::Vec2<float> PanelManager::GetChipCenterPos(const KuroEngine::Vec2<int> _mapchip)
{
	//左上
	KuroEngine::Vec2<float> pos = { _mapchip.x * blockSize + difference.x ,_mapchip.y * blockSize + difference.y };

	//中心にずらす
	pos.x += blockSize * 0.5f;
	pos.y += blockSize * 0.5f;

	return pos;
}

void PanelManager::SetGold()
{
	for (auto& i : onePos) {
		mapchip[i.y][i.x] = int(BlockColor::gold);
	}
}

bool PanelManager::IsPerfect() const
{
	for (auto& y : mapchip) {
		for (auto& x : y) {
			if (x == int(BlockColor::yuka))return false;
		}
	}
	return true;
}
