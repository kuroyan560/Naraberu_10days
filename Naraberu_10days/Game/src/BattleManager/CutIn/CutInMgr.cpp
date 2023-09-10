#include "CutInMgr.h"
#include "FrameWork/WinApp.h"
#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"

void CutInMgr::OnInitialize()
{
}


void CutInMgr::StartCutIn(CutInType Type)
{
	m_NowCutInType = Type;
	CutInTimer = 1;

	using namespace KuroEngine;

	std::string TexDir = "resource/user/tex/battle_scene/";

	switch (Type)
	{
	case CutInType::PLAYER_TURN:
		// 現在次バトル遷移カットイン中の場合は生成しない
		//if (!(NowCutInPtr != nullptr && NowCutInPtr->m_Type == CutInType::NEXT_BATTLE)) {
			NowCutInPtr = std::make_shared<CutInData>(Type, 130, TexDir + "player_turn.png");
		//}
		break;
	case CutInType::ENEMY_TURN:
		NowCutInPtr = std::make_shared<CutInData>(Type, 130, TexDir + "enemy_turn.png");
		break;
	case CutInType::NEXT_BATTLE:
		NowCutInPtr = std::make_shared<CutInData>(Type, 130, TexDir + "next_battle.png");
		break;
	case CutInType::MAX:
		break;
	default:
		break;
	}
}

void CutInMgr::OnUpdate()
{
	// カットイン中であれば
	if (NowCutIn()) {
		CutInTimer < NowCutInPtr->m_CutInEndTime ? CutInTimer++ : CutInTimer = 0;
	}

	// カットイン中ではない場合は何もしない
	if (!NowCutIn()) {
		//NowCutInPtr.reset();
	}
}

void CutInMgr::OnDraw()
{
	// カットイン中ではない場合は何もしない
	if (!NowCutIn()) {
		return;
	}

	using namespace KuroEngine;

	if (m_NowCutInType == CutInType::PLAYER_TURN) {
		float Window_Width = WinApp::Instance()->GetExpandWinSize().x;
		float Tex_Width = 558.0f;
		float ease_value = OutInQuart(float(CutInTimer), 0.0f, 1.0f, float(NowCutInPtr->m_CutInEndTime));
		float Pos = (Window_Width + Tex_Width * 2) - (Window_Width + Tex_Width * 4) * ease_value;
		// 縦
		float Height_Box = 413.0f - 307.0f;
		float Box_Center = 307.0f + Height_Box / 2.0f;
		float Height_Scale_Box = Height_Box * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);
		float Height_Moji = 400.0f - 319.0f;
		float Moji_Center = 319.0f + Height_Moji / 2.0f;
		float Height_Scale_Moji = Height_Moji * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);
		DrawFunc2D::DrawBox2D(Vec2(0.0f, Box_Center - Height_Scale_Box), Vec2(Window_Width, Box_Center + Height_Scale_Box), Color(29, 29, 35, 255), true);
		DrawFunc2D::DrawExtendGraph2D(Vec2(Pos - Tex_Width / 2.0f, Moji_Center - Height_Scale_Moji), Vec2(Pos + Tex_Width / 2.0f, Moji_Center + Height_Scale_Moji), NowCutInPtr->m_Tex[0]);
	}
	else if (m_NowCutInType == CutInType::ENEMY_TURN) {
		float Window_Width = WinApp::Instance()->GetExpandWinSize().x;
		float Tex_Width = 558.0f;
		float ease_value = OutInQuart(float(CutInTimer), 0.0f, 1.0f, float(NowCutInPtr->m_CutInEndTime));
		float Pos = (Window_Width + Tex_Width * 2) - (Window_Width + Tex_Width * 4) * ease_value;
		// 縦
		float Height_Box = 413.0f - 307.0f;
		float Box_Center = 307.0f + Height_Box / 2.0f;
		float Height_Scale_Box = Height_Box * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);
		float Height_Moji = 400.0f - 319.0f;
		float Moji_Center = 319.0f + Height_Moji / 2.0f;
		float Height_Scale_Moji = Height_Moji * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);
		DrawFunc2D::DrawBox2D(Vec2(0.0f, Box_Center - Height_Scale_Box), Vec2(Window_Width, Box_Center + Height_Scale_Box), Color(29, 29, 35, 255), true);
		DrawFunc2D::DrawExtendGraph2D(Vec2(Pos - Tex_Width / 2.0f, Moji_Center - Height_Scale_Moji), Vec2(Pos + Tex_Width / 2.0f, Moji_Center + Height_Scale_Moji), NowCutInPtr->m_Tex[0]);
	}
	else if (m_NowCutInType == CutInType::NEXT_BATTLE) {
		float Window_Width = WinApp::Instance()->GetExpandWinSize().x;
		float Tex_Width = 558.0f;
		float ease_value = OutInQuart(float(CutInTimer), 0.0f, 1.0f, float(NowCutInPtr->m_CutInEndTime));
		float Pos = (Window_Width + Tex_Width * 2) - (Window_Width + Tex_Width * 4) * ease_value;
		// 縦
		float Height_Box = 413.0f - 307.0f;
		float Box_Center = 307.0f + Height_Box / 2.0f;
		float Height_Scale_Box = Height_Box * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);
		float Height_Moji = 400.0f - 319.0f;
		float Moji_Center = 319.0f + Height_Moji / 2.0f;
		float Height_Scale_Moji = Height_Moji * (ease_value < 0.5f ? ease_value : 1.0f - ease_value);
		DrawFunc2D::DrawBox2D(Vec2(0.0f, Box_Center - Height_Scale_Box), Vec2(Window_Width, Box_Center + Height_Scale_Box), Color(29, 29, 35, 255), true);
		DrawFunc2D::DrawExtendGraph2D(Vec2(Pos - Tex_Width / 2.0f, Moji_Center - Height_Scale_Moji), Vec2(Pos + Tex_Width / 2.0f, Moji_Center + Height_Scale_Moji), NowCutInPtr->m_Tex[0]);
	}
}