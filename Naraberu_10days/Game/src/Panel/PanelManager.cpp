#include "PanelManager.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"
#include "../RefreshRate.h"
#include"../Effect/PlayerAttackEffect.h"
#include "../Effect/ScreenShakeManager.h"

KuroEngine::Vec2<int> PanelManager::mapMax;

void PanelManager::Initialize()
{
	//画像読み込み
	std::string TexDir = "resource/user/tex/block/";
	blockTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_pink.png");
	blockTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_blue.png");
	blockTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_yellow.png");
	blockTex[int(BlockColor::yuka)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "yuka.png");
	blockTex[int(BlockColor::obstacle)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "oja.png");
	blockTex[int(BlockColor::eizoku_obstacle)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "eizoku_oja.png");

	//配置場所の範囲決定
	mapchip.resize(10);
	for (auto& i : mapchip) {
		for (int range = 0; range < 10; range++) {
			i.emplace_back(int(BlockColor::yuka));
		}
	}

	massMapchip = mapchip;
	for (auto& y : massMapchip) {
		for (auto& x : y) {
			x = 0;
		}
	}

	mapMax = { int(mapchip.size()), int(mapchip[0].size()) };
}

void PanelManager::Update()
{
	if (isBonusDirection == Bonas::count) {
		BonusCount();
	}
	else if (isBonusDirection == Bonas::add) {
		BonusDirection();
	}
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
			}
		}
	}

	//ボーナスパネル
	if (isBonusDirection != Bonas::add) {return;}

	for (int i = 0; i<int(bonusData.size()); i++) {
		if (i != nowBonusNum) { continue; }

		float dist = bonusEaseScale - blockSize;
		dist = dist / 2.0f;

		for (auto& itr : bonusData[i].pos) {
			//KuroEngine::Vec2<float> pos1 = { itr.x * blockSize + difference.x,itr.y * blockSize + difference.y };
			//KuroEngine::Vec2<float> pos2 = pos1;
			//pos1.x -= blockSize + dist;
			//pos1.y -= blockSize + dist;
			//pos2.x += blockSize + dist;
			//pos2.y += blockSize + dist;

			KuroEngine::Vec2<float> pos = { itr.x * blockSize + difference.x ,itr.y * blockSize + difference.y };

			KuroEngine::Vec2<float> pos1 = pos;
			pos1.x += blockSize;
			pos1.y += blockSize;

			KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1,
			blockTex[int(BlockColor::eizoku_obstacle)], bonusAlpha);
		}
	}
}

void PanelManager::Reset()
{
	for (auto& y : mapchip) {
		for (auto& x : y) {
			if (x == int(BlockColor::eizoku_obstacle)) { continue; }
			x = int(BlockColor::yuka);
		}
	}

	// ボーナスアタック
	PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Bonus_01", 0, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
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

bool PanelManager::JudgeWithEffect(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape, const BlockAttribute _attribute, BlockColor _color, std::weak_ptr<PlayerAttackEffect>_playerAttackEffect)
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
			return false;
		}
	}

	// 設置したらアクション
	if (_attribute == BlockAttribute::attack1) {
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 2, ExistUnits::Instance()->m_NowTarget);
	}
	else if (_attribute == BlockAttribute::attack2) {
		// 強攻撃
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 1);
	}
	else if (_attribute == BlockAttribute::recovery) {

		//とりあえず回復演出ないので直接設置
		for (auto& i : _shape) {
			mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] = int(_color);
		}
		// 回復
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Heal_01", (int)setChipIdxArray.size(), ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
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
			bonusData[count].mass = true;
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
	if (_massMap.y + 1 < mapMax.y && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y + 1][_massMap.x] &&
		massMapchip[_massMap.y + 1][_massMap.x] != 1) {
		MassBlock(_number, _massNum, { _massMap.x,_massMap.y + 1 });
	}
	//上
	if (_massMap.y - 1 >= 0 && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y - 1][_massMap.x] &&
		massMapchip[_massMap.y - 1][_massMap.x] != 1) {
		MassBlock(_number, _massNum, { _massMap.x,_massMap.y - 1 });
	}
	//右
	if (_massMap.x + 1 < mapMax.x && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x + 1] &&
		massMapchip[_massMap.y][_massMap.x + 1] != 1) {
		MassBlock(_number, _massNum, { _massMap.x + 1,_massMap.y });
	}
	//左
	if (_massMap.x - 1 >= 0 && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x - 1] &&
		massMapchip[_massMap.y][_massMap.x - 1] != 1) {
		MassBlock(_number, _massNum, { _massMap.x - 1,_massMap.y });
	}

	//削除場所記録
	if (*_massNum >= 5) {
		KuroEngine::Vec2<int> data = { _massMap.x,_massMap.y };
		bonusData[_number].pos.emplace_back(data);
	}

	return;
}

void PanelManager::LineProcess()
{
	//塊箇所確認
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			if (y == 0) {
				bonusData.emplace_back();
				int size = int(bonusData.size());
				if (!LineBlock(size, { x,y }, false)) {
					bonusData.resize(size - 1);
					continue;
				}
				bonusData[size].mass = false;

			} else if (x == 0) {
				bonusData.emplace_back();
				int size = int(bonusData.size());
				if (!LineBlock(size,{ x,y }, true)) {
					bonusData.resize(size - 1);
					continue;
				}

				bonusData[size].mass = false;
			}
		}
	}
}

bool PanelManager::LineBlock(int _number, const KuroEngine::Vec2<int> _lineMap, const bool _direction)
{
	//下
	if (!_direction) {
		for (int i = 0; i < mapMax.y; i++) {
			if (mapchip[i][_lineMap.x] >= int(BlockColor::yuka)) { return false; }
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
			if (mapchip[_lineMap.y][i] >= int(BlockColor::yuka)) { return false; }
		}

		bonusData.emplace_back();

		for (int i = 0; i < mapMax.y; i++) {
			KuroEngine::Vec2<int> data = { i, _lineMap.y };
			bonusData[_number].pos.emplace_back(data);
		}

	}

	return true;
}

void PanelManager::BonusCount()
{
	bonusData.clear();

	//塊判定
	MassProcess();
	//ライン判定
	LineProcess();

	ExistUnits::Instance()->SetBonusCount(int(bonusData.size()));

	nowBonusNum = 0;
	bonusTimer = 0;
	isBonusDirection=Bonas::add;
}

void PanelManager::BonusDirection()
{
	float maxTimer = 20.0f * RefreshRate::RefreshRate_Mag;

	bonusEaseScale = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Back,
		bonusTimer, maxTimer, blockSize, 60.0f);
	bonusAngle = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Back,
		bonusTimer, maxTimer, 0, 90.0f);
	bonusAlpha = KuroEngine::Math::Ease(KuroEngine::EASE_CHANGE_TYPE::In, KuroEngine::EASING_TYPE::Back,
		bonusTimer, maxTimer, 1.0, 0.0f);

	//時間になったら次に行く
	bonusTimer++;
	if (bonusTimer > maxTimer) {
		nowBonusNum++;
		bonusTimer = 0;
	}
}