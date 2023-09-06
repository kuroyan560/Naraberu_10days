#include "StageManager.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"

KuroEngine::Vec2<int> StageManager::mapMax;

void StageManager::Initialize()
{
	//�摜�ǂݍ���
	std::string TexDir = "resource/user/tex/block/";
	blockTex[int(BlockColor::red)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_pink.png");
	blockTex[int(BlockColor::blue)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_blue.png");
	blockTex[int(BlockColor::yellow)] = KuroEngine::D3D12App::Instance()->GenerateTextureBuffer(TexDir + "block_yellow.png");

	//�z�u�ꏊ�͈̔͌���
	//stagedata�ǂݍ���(�͈͂�mapchip�`���œǂݍ���Ŏg�p����)
	//��������͎�ł�
	mapchip = {
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	};

	massMapchip = mapchip;
	lineMapchip = mapchip;

	mapMax = { int(mapchip.size()), int(mapchip[0].size()) };
}

void StageManager::Update()
{
}

void StageManager::Draw()
{
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			KuroEngine::Vec2<float> pos = { x * blockSize + difference.x ,y * blockSize + difference.y };

			KuroEngine::Vec2<float> pos1 = pos;
			pos1.x += blockSize;
			pos1.y += blockSize;


			if (mapchip[y][x] == 0) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::red)]);
			} else if (mapchip[y][x] == 1) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::blue)]);
			} else if (mapchip[y][x] == 2) {
				KuroEngine::DrawFunc2D::DrawExtendGraph2D(pos, pos1, blockTex[int(BlockColor::yellow)]);
			}
		}
	}
}

void StageManager::Reset()
{
	mapchip = {
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,},
	};
}

#include "PlayerSkills.h"
#include "ExistUnits.h"

bool StageManager::JudgeSet(KuroEngine::Vec2<int> _nowMapchip, std::vector<KuroEngine::Vec2<int>> _shape,
	const BlockAttribute _attribute, BlockColor color)
{
	for (auto& i : _shape) {
		//�X�e�[�W�Ƃ̔��� �_���Ȃ�false
		if (mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] != -1) {
			return false;
		}
	}

	//�͂߂���̂ŐF�ς�
	for (auto& i : _shape) {
		mapchip[_nowMapchip.y + i.y][_nowMapchip.x + i.x] = int(color);
	}

	// �ݒu������A�N�V����
	if (_attribute == BlockAttribute::attack1) {
		// ��U��
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_01", ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	else if (_attribute == BlockAttribute::attack2) {
		// ���U��
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Attack_02", ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	else if (_attribute == BlockAttribute::recovery) {
		// ��
		PlayerSkills::PlayerSkillMgr::Instance()->StartAction("Heal_01", ExistUnits::Instance()->m_pPlayer, ExistUnits::Instance()->m_Enemys[ExistUnits::Instance()->m_NowTarget]);
	}
	return true;
}

bool StageManager::MassProcess(std::vector<int>* _massNum, std::vector<BlockColor>* _color)
{
	//�폜�Ǘ��p�}�b�v������
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			massMapchip[y][x] = 0;
		}
	}

	//��ӏ��m�F
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			//�m�F�ς݂Ȃ玟�ɍs��
			if (massMapchip[y][x] != 0 || mapchip[y][x] == -1) { continue; }
			//��m�F
			int massNum = 0;
			MassBlock(&massNum, { x,y });
			//���5����ۑ�
			if (massNum < 5) { continue; }
			_massNum->emplace_back(massNum);
			_color->emplace_back(BlockColor(mapchip[y][x]));
		}
	}

	return true;
}

void StageManager::MassBlock(int* _massNum, const KuroEngine::Vec2<int> _massMap)
{
	
	//�폜���J�E���g
	*_massNum += 1;
	//�폜�ꏊ�L�^
	massMapchip[_massMap.y][_massMap.x] = 1;

	//��
	if (_massMap.y + 1 < mapMax.y) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y + 1][_massMap.x] &&
			massMapchip[_massMap.y + 1][_massMap.x] != 1) {
			MassBlock(_massNum, { _massMap.x,_massMap.y + 1 });
		}
	}
	//��
	if (_massMap.y - 1 >= 0) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y - 1][_massMap.x] &&
			massMapchip[_massMap.y - 1][_massMap.x] != 1) {
			MassBlock(_massNum, { _massMap.x,_massMap.y - 1 });
		}
	}
	//�E
	if (_massMap.x + 1 < mapMax.x) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x + 1] &&
			massMapchip[_massMap.y][_massMap.x + 1] != 1) {
			MassBlock(_massNum, { _massMap.x + 1,_massMap.y });
		}
	}
	//��
	if (_massMap.x - 1 >= 0) {
		if (mapchip[_massMap.y][_massMap.x] == mapchip[_massMap.y][_massMap.x - 1] &&
			massMapchip[_massMap.y][_massMap.x - 1] != 1) {
			MassBlock(_massNum, { _massMap.x - 1,_massMap.y });
		}
	}

	return;
}

bool StageManager::LineProcess(int* _lineNum, std::vector<BlockColor>* _color)
{
	int lineNum = 0;

	//�폜�Ǘ��p�}�b�v������
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			lineMapchip[y][x] = 0;
		}
	}

	//��ӏ��m�F
	for (int y = 0; y < mapMax.y; y++) {
		for (int x = 0; x < mapMax.x; x++) {
			if (y == 0) {
				if (LineBlock({ x,y }, false) != 1) { continue; }
				_color->emplace_back(BlockColor(mapchip[y][x]));
				lineNum++;

			} else if (x == 0) {
				if (LineBlock({ x,y }, true) != 1) { continue; }
				_color->emplace_back(BlockColor(mapchip[y][x]));
				lineNum++;
			}
		}
	}

	//�L�^
	*_lineNum = lineNum;

	return true;
}

int StageManager::LineBlock(const KuroEngine::Vec2<int> _lineMap, const bool _direction)
{
	//��
	if (!_direction) {
		for (int i = 0; i < mapMax.y; i++) {
			if (mapchip[_lineMap.y][_lineMap.x] != mapchip[i][_lineMap.x] || mapchip[_lineMap.y][_lineMap.x] == -1) { return 0; }
		}

		for (int i = 0; i < mapMax.y; i++) {
			lineMapchip[i][_lineMap.x] = 1;
		}
	}
	//�E
	else {
		for (int i = 0; i < mapMax.x; i++) {
			if (mapchip[_lineMap.y][_lineMap.x] != mapchip[_lineMap.y][i] || mapchip[_lineMap.y][_lineMap.x] == -1) { return 0; }
		}

		for (int i = 0; i < mapMax.x; i++) {
			lineMapchip[_lineMap.y][i] = 1;
		}

	}

	return 1;
}