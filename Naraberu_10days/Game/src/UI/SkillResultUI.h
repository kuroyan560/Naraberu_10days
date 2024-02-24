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

class ParticleEmitter;

class SkillResultUI
{
public:
	enum SKILL
	{
		SKILL_ENEMY_DAMAGE,
		SKILL_PLAYER_HEAL,
		SKILL_ENEMY_DAMAGE_PERFECT,
		SKILL_NUM
	};

private:
	//���ʉ摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, SKILL_NUM>m_skillTex;
	//�f�J�����摜
	std::array<std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10>, SKILL_NUM>m_amountNumTex;
	//���𐔎��摜�i�P�P���Ȃ̂́u+�v���j
	std::array <std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 12>, SKILL_NUM>m_historyNumTex;

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

	KuroEngine::Timer m_expandTimer;
	float m_expand = 1.0f;

	//���݂�UI�\�����W
	KuroEngine::Vec2<float>m_nowPos;

	struct HistoryInfo
	{
		int m_amount = 0;
		bool m_isMul = false;
		float m_alpha = 1.0f;

		HistoryInfo(int arg_amount, bool arg_isMul)
			:m_amount(arg_amount), m_isMul(arg_isMul) {}
	};
	//�^�_���[�W�ő嗚��
	//static const int QUEUE_MAX = 3;
	//�^�_���[�W�����L���[
	std::list<HistoryInfo>m_history;

	bool m_stopDisappear = false;

	KuroEngine::Timer m_disappearDelayTimer;

	void CommonInitOnStart(float arg_appearTime, float arg_disappearDelay);
public:
	SkillResultUI();
	void Init();
	void Update(std::weak_ptr<ParticleEmitter>arg_ultParticleEmitter);
	void Draw();

	void Add(int arg_damage, bool arg_drawHistory, float arg_disappearDelay);
	void Mul(int arg_mulAmount, bool arg_drawHistory, bool arg_isPerfect, float arg_disappearDelay);

	//�o��ʒu�̐ݒ�
	void Set(SKILL arg_skill, KuroEngine::Vec2<float>arg_appearPos, float arg_disappearPosX)
	{
		m_skillType = arg_skill;
		m_appearPos = arg_appearPos;
		m_disappearPosX = arg_disappearPosX;
	}

	//�A�N�e�B�u��Ԃ̃Q�b�^
	const bool& GetIsActive()const { return m_isActive; }

	void SetStopDisappear(bool arg_flg);
};