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
	//�摜�ǂݍ���
	std::string TexDir = "resource/user/tex/block/";
	blockTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_pink.png");
	blockTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_blue.png");
	blockTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_yellow.png");
	blockTex[int(BlockColor::yuka)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "yuka.png");
	blockTex[int(BlockColor::obstacle)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "oja.png");
	blockTex[int(BlockColor::eizoku_obstacle)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "eizoku_oja.png");

	//�z�u�ꏊ�͈̔͌���
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
	//�p�l��
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

	//�{�[�i�X�p�l��
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

	// �{�[�i�X�A�^�b�N
	PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Bonus_01", 0, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[0]);
}

#include "../BattleManager/Player_Act/Skills/PlayerSkills.h"
#include "../BattleManager/ExistUnits.h"

bool PanelManager::JudgeSet(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
	const BlockAttribute _attribute, BlockColor color)
{
	for (auto& i : _shape) {
		//�X�e�[�W�Ƃ̔��� �_���Ȃ�false
		if (mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] != int(BlockColor::yuka)) {
			return false;
		}
	}

	//�͂߂���̂ŐF�ς�
	int Count = 0;
	for (auto& i : _shape) {
		mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] = int(color);
		Count++;
	}

	// �ݒu������A�N�V����
	if (_attribute == BlockAttribute::attack1) {
		// ��U��
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", Count, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	else if (_attribute == BlockAttribute::attack2) {
		// ���U��
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_02", Count, ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	else if (_attribute == BlockAttribute::recovery) {
		// ��
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
	//�܂����o���I����ĂȂ�
	if (_playerAttackEffect.lock()->GetIsActive())return false;

	//�͂߂�C���f�b�N�X�̔z��\��
	std::vector<KuroEngine::Vec2<int>>setChipIdxArray;
	for (auto& i : _shape) {
		setChipIdxArray.emplace_back(_nowMapchip + i);
	}

	for (auto& i : setChipIdxArray) {
		//�X�e�[�W�Ƃ̔��� �_���Ȃ�false
		if (mapchip[i.y][i.x] != int(BlockColor::yuka)) {
			return false;
		}
	}

	// �ݒu������A�N�V����
	if (_attribute == BlockAttribute::attack1) {
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 2, ExistUnits::Instance()->m_NowTarget);
	}
	else if (_attribute == BlockAttribute::attack2) {
		// ���U��
		_playerAttackEffect.lock()->Start(setChipIdxArray, _color, 1);
	}
	else if (_attribute == BlockAttribute::recovery) {

		//�Ƃ肠�����񕜉��o�Ȃ��̂Œ��ڐݒu
		for (auto& i : _shape) {
			mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] = int(_color);
		}
		// ��
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Heal_01", (int)setChipIdxArray.size(), ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	return true;
}

void PanelManager::MassProcess()
{
	//�폜�Ǘ��p�}�b�v������
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			massMapchip[y][x] = 0;
		}
	}

	//��ӏ��m�F
	int count = 0;
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			//�m�F�ς݂Ȃ玟�ɍs��
			if (massMapchip[y][x] != 0 || mapchip[y][x] >= int(BlockColor::yuka)) { continue; }
			//��m�F
			int massNum = 0;
			bonusData.emplace_back();
			MassBlock(count, &massNum, { x,y });
			//���5����ۑ�
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
	//�폜���J�E���g
	*_massNum += 1;
	massMapchip[_massMap.y][_massMap.x] = 1;

	//��
	if (_massMap.y + 1 < mapMax.y && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y + 1][_massMap.x] &&
		massMapchip[_massMap.y + 1][_massMap.x] != 1) {
		MassBlock(_number, _massNum, { _massMap.x,_massMap.y + 1 });
	}
	//��
	if (_massMap.y - 1 >= 0 && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y - 1][_massMap.x] &&
		massMapchip[_massMap.y - 1][_massMap.x] != 1) {
		MassBlock(_number, _massNum, { _massMap.x,_massMap.y - 1 });
	}
	//�E
	if (_massMap.x + 1 < mapMax.x && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x + 1] &&
		massMapchip[_massMap.y][_massMap.x + 1] != 1) {
		MassBlock(_number, _massNum, { _massMap.x + 1,_massMap.y });
	}
	//��
	if (_massMap.x - 1 >= 0 && mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x - 1] &&
		massMapchip[_massMap.y][_massMap.x - 1] != 1) {
		MassBlock(_number, _massNum, { _massMap.x - 1,_massMap.y });
	}

	//�폜�ꏊ�L�^
	if (*_massNum >= 5) {
		KuroEngine::Vec2<int> data = { _massMap.x,_massMap.y };
		bonusData[_number].pos.emplace_back(data);
	}

	return;
}

void PanelManager::LineProcess()
{
	//��ӏ��m�F
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
	//��
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
	//�E
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

	//�򔻒�
	MassProcess();
	//���C������
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

	//���ԂɂȂ����玟�ɍs��
	bonusTimer++;
	if (bonusTimer > maxTimer) {
		nowBonusNum++;
		bonusTimer = 0;
	}
}