#include "EnemyDamageUI.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"KuroEngine.h"

EnemyDamageUI::EnemyDamageUI()
{
	using namespace KuroEngine;

	std::string dir = "resource/user/tex/battle_scene/";
	//�uDamage�v
	m_damageTex = D3D12App::Instance()->GenerateTextureBuffer(dir + "damage.png");
	//�^�_���[�W����
	D3D12App::Instance()->GenerateTextureBuffer(
		m_damageNumTex.data(), dir + "damage_number.png", 10, Vec2(10, 1));
	//�^�_���[�W���𐔎�
	D3D12App::Instance()->GenerateTextureBuffer(
		m_damageHisNumTex.data(), dir + "damage_rireki_number.png", 11, Vec2(11, 1));
}

void EnemyDamageUI::Init(KuroEngine::Vec2<float>arg_appearPos)
{
	m_appearPos = arg_appearPos;
	m_isActive = false;
	m_impactShake.Init();
}

void EnemyDamageUI::Update()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	m_impactShake.Update(1.0f);

	//�ޏ��̍��WX
	const float DISAPPEAR_POS_X = 1500.0f;

	//�ޏ�J�n�܂ł̎���
	if (m_appearTimer.UpdateTimer())
	{
		//�ޏꒆ
		m_disappearTimer.UpdateTimer();
		m_nowPos.x = Math::Ease(In, Back, m_disappearTimer.GetTimeRate(), m_appearPos.x, DISAPPEAR_POS_X);

		//�ޏꊮ��
		if (m_disappearTimer.IsTimeUp())
		{
			m_isActive = false;
		}
	}
}

void EnemyDamageUI::Draw()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	const Vec2<float>NUMBER_OFFSET_POS = { 0.0f,-57.0f };

	Vec2<float>shake = { 0.0f,m_impactShake.GetOffset().y };
	auto pos = m_nowPos + shake;
	DrawFunc2D::DrawRotaGraph2D(pos, {1.0f,1.0f}, 0.0f, m_damageTex);
	DrawFunc2D::DrawNumber2D(m_damageAmount, pos + NUMBER_OFFSET_POS, m_damageNumTex.data(), { 1.0f,1.0f },
		0.0f, HORIZONTAL_ALIGN::CENTER, VERTICAL_ALIGN::CENTER, 2);
}

void EnemyDamageUI::Add(int arg_damage)
{
	//�U���p�����[�^
	const float SHAKE_TIME = 15.0f;
	const float SHAKE_SPAN = 2.0f;
	const float SHAKE_POWER_MIN = 6.0f;
	const float SHAKE_POWER_MAX = 18.0f;

	//�\������
	const float APPEAR_TIME = 70.0f;
	m_appearTimer.Reset(APPEAR_TIME);

	//�ޏꎞ��
	const float DISAPPEAR_TIME = 30.0f;
	m_disappearTimer.Reset(DISAPPEAR_TIME);

	//��\�����
	if (!m_isActive)
	{
		m_damageAmount = 0;
	}

	m_isActive = true;
	m_damageAmount += arg_damage;
	m_impactShake.Shake(SHAKE_TIME, SHAKE_SPAN, SHAKE_POWER_MIN, SHAKE_POWER_MAX);
	m_nowPos = m_appearPos;
}