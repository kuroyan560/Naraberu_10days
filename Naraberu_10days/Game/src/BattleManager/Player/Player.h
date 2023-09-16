#pragma once
#include <array>
#include "../UnitBase.h"

#include "DirectX12/D3D12App.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "FrameWork/WinApp.h"

#include "../../RefreshRate.h"

#include<memory>
class UltActivateEffect;

class Player :
    public UnitBase
{
private:

	// 画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_break;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Ult_Gauge;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 12> m_NumberTex;

	//キャラ画像
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterDamageTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterMabatakiTex;
	// まばたき用タイマー
	int m_Player_Mabataki_Timer;
public:
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
private:
	int m_Damage_Timer;
public:
	// ダメージを受ける
	void Damage(int value)override;

	// プレイヤーターン終了前処理
	void TurnEnd_BeforeTurnChange();

	// 全ステータスをセットする
	void SetState(int HP, int MaxHP);

	// ウルトが溜まった時の演出
	void UltMaxEffect();

	// ターンエンド
	bool m_IsEndBonus;
	bool m_DoBonus;
	void TurnEndTrigger();

	// アルティメット関連
private:
	// 現在のアルティメットポイントの値
	int m_UltimatePoint;
	// 変化前のアルティメットポイント
	int m_BeforeUltimatePoint;
	// アルティメットポイントの最大値
	int Max_UltimatePoint;

	//アルティメットの発動演出
	std::weak_ptr<UltActivateEffect>m_UltimateActivateEffect;

public:
	// ウルト演出のポインタ
	std::shared_ptr<UltActivateEffect> GetUltEffectPtr() { return m_UltimateActivateEffect.lock(); }
	// アルティメットポイントを加算する
	void AddUltPoint(int Amount) {
		// 変化前の値を保存
		m_BeforeUltimatePoint = m_UltimatePoint;
		// アルティメットポイントを加算
		m_UltimatePoint += Amount;
		m_UltimatePoint > Max_UltimatePoint ? m_UltimatePoint = Max_UltimatePoint : 0;
	}
	// アルティメットポイントを減算する
	void SubUltPoint(int Amount) {
		// 変化前の値を保存
		m_BeforeUltimatePoint = m_UltimatePoint;
		// アルティメットポイントを加算
		m_UltimatePoint -= Amount;
		m_UltimatePoint < 0 ? m_UltimatePoint = 0 : 0;
	}
	// 現在のアルティメットポイントの取得
	int GetUltPoint() { return m_UltimatePoint; }
	// 現在のアルティメットのチャージ率を取得
	float GetUltRate() { return float(m_UltimatePoint) / float(Max_UltimatePoint); }
	// アルティメットが溜まっているかどうか
	bool IsReadyUltimate() { return m_UltimatePoint >= Max_UltimatePoint; }

	void EndBonusAttack() { m_IsEndBonus = true; }
	bool GetEndBonusAttack() { return m_IsEndBonus; }

	void SetUltActivateEffect(std::shared_ptr<UltActivateEffect>Effect) { m_UltimateActivateEffect = Effect; }

	// 描画位置関連
private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;

	std::array<KuroEngine::Vec2<float>,4> ultSize;


public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }
};

