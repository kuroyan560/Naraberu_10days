#pragma once
#include "KuroEngineDevice.h"

#include "../BattleManager/Player/Player.h" 
#include "../BattleManager/Enemy/Enemy.h"
#include "../BattleManager/BattleTurnMgr.h"
#include "../Block/BlockManager.h"
#include "../Panel/PanelManager.h"
#include "../Stage/StageList.h"
#include "../Fade/Fade.h"
#include"../Effect/SetPrismEffect.h"
#include "../UI/PlayerDamageUi.h"
#include "../Effect/UltActivateEffect.h"
#include "../Effect/BackGroundEffect.h"
#include"../Effect/PerfectBonusEffect.h"

class ParticleEmitter;

class BattleScene :
    public KuroEngine::BaseScene
{
	// プレイヤー
	std::shared_ptr<Player> Pl;
	// 敵リスト
	std::vector<std::shared_ptr<UnitBase>> En;
	// バトル
	BattleTurnMgr Mgr;
	// 背景
	std::shared_ptr<KuroEngine::TextureBuffer> m_SukasiTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_BackTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_StageTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_BattleTex;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>,12> m_NumberTex;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 12> m_NumberTex_Battle;
	//ブロック
	std::shared_ptr<BlockManager> block;
	//ステージ
	std::shared_ptr<PanelManager> stage;

	BlockAttribute attribute;

	// ステージクリア
	std::shared_ptr<KuroEngine::TextureBuffer> m_ClearTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_MaxComboTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TotalBonusTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_TotalTurnTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_KeyTex_Clear;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_ControllerTex_Clear;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DoneTex_Clear;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 10> m_ClearNumberTex;

	// ゲームオーバー
	// 0 : リトライ 1 : ステージセレクト
	int m_GameOverSelectIndex;
	bool m_Already_Selected;
	std::shared_ptr<KuroEngine::TextureBuffer> m_GameoverTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_RetryTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_StageSelectTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_SelectTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_KeyTex_GameOver;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_ControllerTex_GameOver;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DoneTex_GameOver;

	// フェード
	Fade m_Fade;

	std::shared_ptr<ParticleEmitter>m_healPtEmitter;
	std::shared_ptr<ParticleEmitter>m_backPrismPtEmitter;
	std::shared_ptr<ParticleEmitter>m_ultPtEmitter;

	//背景
	std::shared_ptr<BackGroundEffect> backGroundEffect;

	//（移動する）
	//プレイヤーのダメージ
	std::shared_ptr<PlayerDamageUi> dame;

	void PlayerTurn();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

	// プリズムを設置出来ないようにするタイマー
	int m_Impossible_Put_Block_Timer;
	// プリズムを設置出来ないようにする時間
	int m_Impossible_Put_Block_Effect_Time;

	// 現在のステージ
	Stages::StageData m_NowStage;
	// 現在のウェーブ
	int m_NowWave;
	// ステージをセット
	void SetStage(std::string StageName);
	// 次のウェーブスタート
	void NextWave();
	// 全ウェーブ終了したか
	bool m_Stage_End;
	// ステージ終了演出用
	int m_End_Timer;
	int m_End_Timer_Finish;


	// ゲームクリア描画
	void GameClearDraw();
	// ゲームオーバー画面
	void GameOverDraw();
	// ポーズ画面
	void PauseDraw();
	
	// リザルト表示用
	float ResultTimer;
	float ResultTimer_Max;
	float ResultEasing(float time);

	int m_NowTutorial_Step;
	static const int m_Tutorial_Step_Max = 36;
	std::array<bool, m_Tutorial_Step_Max> m_Tutorial_Pause;
	// 該当する操作入力をした時に何フレーム後に次に移るかのタイマー
	int m_Tutorial_Trigger_Timer;
	// 無操作状態のタイマー
	int m_Tutorial_None_Ctrl_Timer;
	// 無操作状態時テキストタイマー
	int m_Tutorial_Jissen_Text_Timer;
	int m_Tutorial_Jissen_Text_Timer_Max;
	// 2度操作がある場合のフラグ
	bool m_Tutorial_Step_Complete_Flag;
	// 置いたタイマー
	int m_PutTimer;
	// 注目タイマー
	int m_TaegetTimer;
	int m_TaegetTimer_Max;
	KuroEngine::Vec2<float> m_LastTarget_LT;
	KuroEngine::Vec2<float> m_LastTarget_RB;
	KuroEngine::Vec2<float> m_NowTarget_LT;
	KuroEngine::Vec2<float> m_NowTarget_RB;
	bool m_NowTargeting;
	int m_Taeget_Alpha_Timer;
	float InQuart(float t) {
		return t * t * t * t;
	}
	float OutQuart(float t) {
		return 1.0f - powf(1.0f - t, 4.0f);
	}
	float OutQuint(float t) {
		return 1.0f - powf(1.0f - t, 5.0f);
	}
	float EaseFunc(float start, float end, float Now, float Max) {
		float t = Now / Max;
		float result = start + static_cast<float>(OutQuart(t) * static_cast<float>(end - start));
		return result;
	}
	KuroEngine::Vec2<float> TargetSize(KuroEngine::Vec2<float> St, KuroEngine::Vec2<float> En);
	// メッセージ送りクールタイム
	int m_Tutorial_Text_CoolTime;
	std::shared_ptr<KuroEngine::TextureBuffer> Tutorial_Back_Tex;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 36> Tutorial_Tex;
	void TutorialUpdate();
	void TutorialDraw();

	// ポーズ用
	bool m_IsPause;
	// 0 : 再開 1 : リトライ 2 : ステージセレクトへ
	int m_PauseMenu;
	bool m_Already_Selected_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_PauseTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_ResumeTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_RetryTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_StageSelectTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_SelectCosorTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_KeyTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_Done_ControllerTex_Pause;
	std::shared_ptr<KuroEngine::TextureBuffer> m_DoneTex_Pause;

	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Done;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Pass;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Left;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Right;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Set;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_TurnEnd;
	std::array<std::shared_ptr<KuroEngine::TextureBuffer>, 2> m_Operation_Ult;

	//プリズム設置演出
	std::shared_ptr<SetPrismEffect>m_setPrismEffect;

	//ULT発動演出
	std::shared_ptr<UltActivateEffect>m_ultActivateEffect;

	//パーフェクト演出
	std::shared_ptr<PerfectBonusEffect>m_perfectBonusEffect;

	//敵に対する与ダメージUI
	static const int ENEMY_COUNT_MAX = 3;
	std::vector<std::shared_ptr<SkillResultUI>>m_enemyDamageUI;
	std::vector<std::weak_ptr<SkillResultUI>>GetAliveEnemyDamageUIArray();

public:
	BattleScene();

};

static DirectX::XMFLOAT3 hsv2rgb(DirectX::XMFLOAT3 hsv)
{
	DirectX::XMFLOAT3  rgb = XMFLOAT3();
	int     Hi;
	float   f;
	float   p;
	float   q;
	float   t;

	Hi = int(fmod(floor(hsv.x / 60.0f), 6.0f));
	f = hsv.x / 60.0f - Hi;
	p = hsv.z * (1.0f - hsv.y);
	q = hsv.z * (1.0f - f * hsv.y);
	t = hsv.z * (1.0f - (1.0f - f) * hsv.y);

	if (Hi == 0) {
		rgb.x = hsv.z;
		rgb.y = t;
		rgb.z = p;
	}
	if (Hi == 1) {
		rgb.x = q;
		rgb.y = hsv.z;
		rgb.z = p;
	}
	if (Hi == 2) {
		rgb.x = p;
		rgb.y = hsv.z;
		rgb.z = t;
	}
	if (Hi == 3) {
		rgb.x = p;
		rgb.y = q;
		rgb.z = hsv.z;
	}
	if (Hi == 4) {
		rgb.x = t;
		rgb.y = p;
		rgb.z = hsv.z;
	}
	if (Hi == 5) {
		rgb.x = hsv.z;
		rgb.y = p;
		rgb.z = q;
	}

	return rgb;
}