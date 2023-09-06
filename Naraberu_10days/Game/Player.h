#pragma once
#include "UnitBase.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "../../src/engine/FrameWork/WinApp.h"

#include "RefreshRate.h"

class Player :
    public UnitBase
{
private:

	// 画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;

	//キャラ画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterTex;

	int TurnChangeTimer;
	int TurnChangeTime_Fin;

public:
	Player();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnAlwaysUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

	// プレイヤーターン終了前処理
	void TurnEnd_BeforeTurnChange();

	// 全ステータスをセットする
	void SetState(int HP, int MaxHP);

	// アルティメット関連
private:
	// 現在のアルティメットポイントの値
	int m_UltimatePoint;
	// 変化前のアルティメットポイント
	int m_BeforeUltimatePoint;
	// アルティメットポイントの最大値
	int Max_UltimatePoint;

public:
	// アルティメットポイントを加算する
	void AddUltPoint(int Amount) {
		// 変化前の値を保存
		m_BeforeUltimatePoint = m_UltimatePoint;
		// アルティメットポイントを加算
		m_UltimatePoint += Amount;
		m_UltimatePoint > Max_UltimatePoint ? m_UltimatePoint = Max_UltimatePoint : 0;
	}
	// 現在のアルティメットポイントの取得
	int GetUltPoint() { return m_UltimatePoint; }
	// 現在のアルティメットのチャージ率を取得
	float GetUltRate() { return float(m_UltimatePoint) / float(Max_UltimatePoint); }
	// アルティメットが溜まっているかどうか
	bool IsReadyUltimate() { return m_UltimatePoint >= Max_UltimatePoint; }

	// 描画位置関連
private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;
public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }
};

