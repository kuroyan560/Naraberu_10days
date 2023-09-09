#include "Reticle.h"
#include "../BattleTurnMgr.h"
#include "../UnitBase.h"
#include "../Enemy/Enemy.h"
#include "../../src/engine/DirectX12/D3D12App.h"

#include "../../Effect/ScreenShakeManager.h"

Reticle::Reticle()
{
	using namespace KuroEngine;
	std::string TexDir = "resource/user/tex/battle_scene/";
	m_BigReticleTex = D3D12App::Instance()->GenerateTextureBuffer(TexDir + "reticle_big.png");
	D3D12App::Instance()->GenerateTextureBuffer(&m_ReticleTex.front(), TexDir + "reticle.png", 4, Vec2(4, 1));
	MaskColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
}

void Reticle::SetBattleTurnManager(BattleTurnMgr* ptr)
{
	m_pBTM = ptr;
}

void Reticle::Update()
{
	// タイマー加算
	m_Reticle_Timer += 1.0f / RefreshRate::RefreshRate_Mag;

	if (Reticle::Instance()->m_CanMove) {
		// ロックオン
		if (OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_UP)) {
			// 生きているユニットまで
			bool ChangeTargetSuccess = false;
			for (int i = ExistUnits::Instance()->m_NowTarget; i > 0; i--) {
				if (m_pBTM->UnitList[i]->IsAlive()) {
					ExistUnits::Instance()->m_NowTarget = i - 1;
					ChangeTargetSuccess = true;
					m_Reticle_Timer = 0.0f;
					break;
				}
			}
			// ターゲットを変更できなかった
			if (!ChangeTargetSuccess) {

			}
		}
		if (OperationConfig::Instance()->GetTargetChangeVec(OperationConfig::SELECT_VEC_DOWN)) {
			// 生きているユニットまで
			bool ChangeTargetSuccess = false;
			for (int i = ExistUnits::Instance()->m_NowTarget + 1; i < m_pBTM->UnitList.size() - 1; i++) {
				if (m_pBTM->UnitList[i + 1]->IsAlive()) {
					ExistUnits::Instance()->m_NowTarget = i;
					ChangeTargetSuccess = true;
					m_Reticle_Timer = 0.0f;
					break;
				}
			}
			// ターゲットを変更できなかった
			if (!ChangeTargetSuccess) {

			}
		}
		// ターゲット中の敵が死んでいる場合
		if (!m_pBTM->UnitList[ExistUnits::Instance()->m_NowTarget + 1]->IsAlive()) {
			// 一番上の生きている敵を狙う
			for (int i = 1; i < m_pBTM->UnitList.size(); i++) {
				if (m_pBTM->UnitList[i]->IsAlive()) {
					ExistUnits::Instance()->m_NowTarget = i - 1;
					break;
				}
			}
		}
	}

	/*for (auto& data : m_Aura) {
		data.Update();
	}*/
}

void Reticle::Draw(int Index, KuroEngine::Vec2<float> LT, KuroEngine::Vec2<float> RB, KuroEngine::Color Mask)
{
	using namespace KuroEngine;

	const float pi = 3.14159265358979323846264338327950288f;
	//float ss = sin((m_Reticle_Timer * pi) / 180.0f);
	float bairitu = m_Reticle_Timer / 10.0f;
	float Size_X = 22 * (bairitu > 1.0f ? 1.0f : bairitu);
	float Size_Y = 35 * (bairitu > 1.0f ? 1.0f : bairitu);
	// 上下両方のユニットが生きている
	if (IsAliveUpperUnit() && IsAliveUnderUnit()) {
		DrawFunc2D_Color::DrawExtendGraph2D(Vec2(LT.x - Size_X, RB.y - Size_Y) + ScreenShakeManager::Instance()->GetOffset()
			, Vec2(LT.x + Size_X, RB.y + Size_Y) + ScreenShakeManager::Instance()->GetOffset()
			, m_ReticleTex[1], Mask,
			{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	}
	// 上のユニットのみ生きている
	else if (IsAliveUpperUnit()) {
		DrawFunc2D_Color::DrawExtendGraph2D(Vec2(LT.x - Size_X, RB.y - Size_Y) + ScreenShakeManager::Instance()->GetOffset()
			, Vec2(LT.x + Size_X, RB.y + Size_Y) + ScreenShakeManager::Instance()->GetOffset(), m_ReticleTex[2], Mask,
			{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	}
	// 下のユニットのみ生きている
	else if (IsAliveUnderUnit()) {
		DrawFunc2D_Color::DrawExtendGraph2D(Vec2(LT.x - Size_X, RB.y - Size_Y) + ScreenShakeManager::Instance()->GetOffset()
			, Vec2(LT.x + Size_X, RB.y + Size_Y) + ScreenShakeManager::Instance()->GetOffset(), m_ReticleTex[0], Mask,
			{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	}
	// 自分だけ生きてる
	else {
		DrawFunc2D_Color::DrawExtendGraph2D(Vec2(LT.x - Size_X, RB.y - Size_Y) + ScreenShakeManager::Instance()->GetOffset()
			, Vec2(LT.x + Size_X, RB.y + Size_Y ) + ScreenShakeManager::Instance()->GetOffset(), m_ReticleTex[3], Mask,
			{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);
	}
	DrawFunc2D::DrawRotaGraph2D(Vec2(LT.x, RB.y) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(0.75f, 0.75f), m_Reticle_Timer / 60.0f, m_BigReticleTex, 0.2f);
	DrawFunc2D::DrawRotaGraph2D(Vec2(LT.x, RB.y) + ScreenShakeManager::Instance()->GetOffset()
		, Vec2(0.65f, 0.65f), -m_Reticle_Timer / 63.0f, m_BigReticleTex, 0.2f);

	MaskColor = Mask;

	/*for (auto& data : m_Aura) {
		data.Draw();
	}*/
}

bool Reticle::IsAliveUpperUnit()
{
	bool ChangeTargetSuccess = false;
	for (int i = ExistUnits::Instance()->m_NowTarget; i > 0; i--) {
		if (m_pBTM->UnitList[i]->IsAlive()) {
			ChangeTargetSuccess = true;
			break;
		}
	}
	return ChangeTargetSuccess;
}

bool Reticle::IsAliveUnderUnit()
{
	bool ChangeTargetSuccess = false;
	for (int i = ExistUnits::Instance()->m_NowTarget + 1; i < m_pBTM->UnitList.size() - 1; i++) {
		if (m_pBTM->UnitList[i + 1]->IsAlive()) {
			ChangeTargetSuccess = true;
			break;
		}
	}
	return ChangeTargetSuccess;
}

void Reticle::Reticle_Aura::SetPoint(UnitBase* ptr, bool Reverse)
{
	//UnitPtr = ptr;
	//m_Reverse = Reverse;
	/*using namespace KuroEngine;
	Point.emplace_back(Vec2<float>(LT.x, RB.y));
	Point.emplace_back(Vec2<float>(RB.x, RB.y));
	Point.emplace_back(Vec2<float>(RB.x, LT.y));
	Point.emplace_back(Vec2<float>(LT.x, LT.y));
	Point.emplace_back(Vec2<float>(LT.x, RB.y));
	if (Reverse) {
		std::reverse(Point.begin(), Point.end());
	}*/
}

void Reticle::Reticle_Aura::Update()
{
	/*using namespace KuroEngine;
	Vec2<float> LT = GetUnitPtr_nama<Enemy*>(UnitPtr)->Get_Left_Top();
	Vec2<float> RB = GetUnitPtr_nama<Enemy*>(UnitPtr)->Get_Right_Bottom();
	Points.emplace_back(Vec2<float>(LT.x, RB.y));
	Points.emplace_back(Vec2<float>(RB.x, RB.y));
	Points.emplace_back(Vec2<float>(RB.x, LT.y));
	Points.emplace_back(Vec2<float>(LT.x, LT.y));
	Points.emplace_back(Vec2<float>(LT.x, RB.y));
	if (m_Reverse) {
		std::reverse(Points.begin(), Points.end());
	}

	m_Timer += 1.0f / RefreshRate::RefreshRate_Mag;

	m_Position = Reticle::Instance()->getCurrentPosition(Points, 100, &Index,int(m_Timer));*/
}

void Reticle::Reticle_Aura::Draw()
{
	using namespace KuroEngine;
	/*DrawFunc2D_Color::DrawExtendGraph2D(m_Position - Vec2(20.0f, 20.0f), m_Position + Vec2(20.0f, 20.0f),
		Reticle::Instance()->m_ReticleTex[3], Reticle::Instance()->MaskColor,
		{ false,false }, { 0.0f,0.0f }, { 1.0f,1.0f }, KuroEngine::DrawFunc2D_Color::FILL_MDOE::MUL);*/
}
