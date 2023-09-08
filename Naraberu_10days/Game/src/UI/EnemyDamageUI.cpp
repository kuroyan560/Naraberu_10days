#include "EnemyDamageUI.h"
#include"DirectX12/D3D12App.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"
#include"KuroEngine.h"

EnemyDamageUI::EnemyDamageUI()
{
	using namespace KuroEngine;

	std::string dir = "resource/user/tex/battle_scene/";
	//「Damage」
	m_damageTex = D3D12App::Instance()->GenerateTextureBuffer(dir + "damage.png");
	//与ダメージ数字
	D3D12App::Instance()->GenerateTextureBuffer(
		m_damageNumTex.data(), dir + "damage_number.png", 10, Vec2(10, 1));
	//与ダメージ履歴数字
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

	//退場後の座標X
	const float DISAPPEAR_POS_X = 1500.0f;

	//退場開始までの時間
	if (m_appearTimer.UpdateTimer())
	{
		//退場中
		m_disappearTimer.UpdateTimer();
		m_nowPos.x = Math::Ease(In, Back, m_disappearTimer.GetTimeRate(), m_appearPos.x, DISAPPEAR_POS_X);

		//退場完了
		if (m_disappearTimer.IsTimeUp())
		{
			//履歴削除
			m_history.clear();
			m_isActive = false;
		}
	}
}

void EnemyDamageUI::Draw()
{
	using namespace KuroEngine;

	if (!m_isActive)return;

	//ダメージ数描画オフセット座標
	const Vec2<float>NUMBER_OFFSET_POS = { 0.0f,-57.0f };

	Vec2<float>shake = { 0.0f,m_impactShake.GetOffset().y };
	auto pos = m_nowPos + shake;
	DrawFunc2D::DrawRotaGraph2D(pos, {1.0f,1.0f}, 0.0f, m_damageTex);
	DrawFunc2D::DrawNumber2D(m_damageAmount, pos + NUMBER_OFFSET_POS,
		m_damageNumTex.data(), { 1.0f,1.0f }, 1.0f, 0.0f,
		HORIZONTAL_ALIGN::CENTER, VERTICAL_ALIGN::CENTER, 2);

	//最新のダメージ履歴の描画オフセット座標
	const Vec2<float>DAMAGE_HIS_OFFSET_POS = { 40.0f,22.0f };
	//履歴間の行間
	const float DAMAGE_HIS_LINE_SPACE = 29.0f;

	auto damageHisPos = m_appearPos + shake + DAMAGE_HIS_OFFSET_POS;
	for (auto itr = m_history.begin(); itr != m_history.end(); ++itr)
	{
		size_t idx = std::distance(m_history.begin(), itr);
		float alpha = 1.0f - static_cast<float>(idx) / m_history.size();
		alpha *= 1.0f - m_appearTimer.GetTimeRate();

		DrawFunc2D::DrawNumber2D(*itr, damageHisPos,
			m_damageHisNumTex.data(), { 1.0f,1.0f }, alpha, 0.0f, HORIZONTAL_ALIGN::RIGHT,
			VERTICAL_ALIGN::TOP, -1, 10, -1);

		//行間ずらし
		damageHisPos.y += DAMAGE_HIS_LINE_SPACE;
	}
}

void EnemyDamageUI::Add(int arg_damage)
{
	//振動パラメータ
	const float SHAKE_TIME = 15.0f;
	const float SHAKE_SPAN = 2.0f;
	const float SHAKE_POWER_MIN = 6.0f;
	const float SHAKE_POWER_MAX = 18.0f;

	//表示時間
	const float APPEAR_TIME = 70.0f;
	m_appearTimer.Reset(APPEAR_TIME);

	//退場時間
	const float DISAPPEAR_TIME = 30.0f;
	m_disappearTimer.Reset(DISAPPEAR_TIME);

	//与ダメージ最大履歴数
	const int QUEUE_MAX = 3;

	//非表示状態
	if (!m_isActive)
	{
		m_damageAmount = 0;
	}

	m_isActive = true;
	m_damageAmount += arg_damage;
	m_impactShake.Shake(SHAKE_TIME, SHAKE_SPAN, SHAKE_POWER_MIN, SHAKE_POWER_MAX);
	m_nowPos = m_appearPos;

	m_history.push_front(arg_damage);
	if (QUEUE_MAX < static_cast<int>(m_history.size()))
	{
		m_history.pop_back();
	}
}