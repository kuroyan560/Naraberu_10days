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

class EnemyDamageUI
{
	//�uDamage�v�摜
	std::shared_ptr<KuroEngine::TextureBuffer>m_damageTex;
	//�_���[�W�����摜
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10>m_damageNumTex;
	//�_���[�W���𐔎��摜�i�P�P���Ȃ̂́u+�v���j
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 11>m_damageHisNumTex;

	//�U��
	KuroEngine::ImpactShake m_impactShake;

	//�o���^�C�}�[
	KuroEngine::Timer m_appearTimer;

	//�ޏ�^�C�}�[
	KuroEngine::Timer m_disappearTimer;

	bool m_isActive = false;
	int m_damageAmount = 0;

	//UI���������W
	KuroEngine::Vec2<float>m_appearPos = { 200.0f,200.0f };

	//���݂�UI�\�����W
	KuroEngine::Vec2<float>m_nowPos;

	//�^�_���[�W�����L���[
	std::list<std::pair<int, float>>m_history;

public:
	EnemyDamageUI();
	void Init();
	void Update();
	void Draw();

	void Add(int arg_damage, bool arg_drawHistory);

	//�o��ʒu�̐ݒ�
	void SetAppearPos(KuroEngine::Vec2<float>arg_appearPos)
	{
		m_appearPos = arg_appearPos;
	}

	//�A�N�e�B�u��Ԃ̃Q�b�^
	const bool& GetIsActive()const { return m_isActive; }
};