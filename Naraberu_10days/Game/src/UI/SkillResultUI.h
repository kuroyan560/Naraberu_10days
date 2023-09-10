#pragma once
#include<array>
#include<memory>
#include"Common/Vec.h"
#include<vector>
#include"ForUser/Timer.h"
#include"ForUser/ImpactShake.h"
#include<list>

namespace KuroEngine
{
	class TextureBuffer;
};

class SkillResultUI
{
public:
	enum SKILL
	{
		SKILL_ENEMY_DAMAGE,
		SKILL_PLAYER_HEAL,
		SKILL_NUM
	};

private:
	//���ʉ摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, SKILL_NUM>m_skillTex;
	//�f�J�����摜
	std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10>, SKILL_NUM>m_amountNumTex;
	//���𐔎��摜�i�P�P���Ȃ̂́u+�v���j
	std::array <std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11>, SKILL_NUM>m_historyNumTex;

	//�U��
	KuroEngine::ImpactShake m_impactShake;

	//�o���^�C�}�[
	KuroEngine::Timer m_appearTimer;

	//�ޏ�^�C�}�[
	KuroEngine::Timer m_disappearTimer;

	//�A�N�e�B�u���
	bool m_isActive = false;

	//�X�L�����
	SKILL m_skillType;

	//����
	int m_amount = 0;

	//UI���������W
	KuroEngine::Vec2<float>m_appearPos = { 200.0f,200.0f };
	//UI�ޏ���X���W
	float m_disappearPosX = 0.0f;


	//���݂�UI�\�����W
	KuroEngine::Vec2<float>m_nowPos;

	//�^�_���[�W�����L���[
	std::list<std::pair<int, float>>m_history;

public:
	SkillResultUI();
	void Init();
	void Update();
	void Draw();

	void Add(int arg_damage, bool arg_drawHistory);

	//�o��ʒu�̐ݒ�
	void Set(SKILL arg_skill, KuroEngine::Vec2<float>arg_appearPos, float arg_disappearPosX)
	{
		m_skillType = arg_skill;
		m_appearPos = arg_appearPos;
		m_disappearPosX = arg_disappearPosX;
	}

	//�A�N�e�B�u��Ԃ̃Q�b�^
	const bool& GetIsActive()const { return m_isActive; }
};