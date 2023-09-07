#pragma once
#include "KuroEngineDevice.h"

#include "../BattleManager/Player/Player.h" 
#include "../BattleManager/Enemy/Enemy.h"
#include "../BattleManager/BattleTurnMgr.h"
#include "../Block/BlockManager.h"
#include "../Panel/PanelManager.h"
#include "../Stage/StageList.h"

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
	//ブロック
	std::shared_ptr<BlockManager> block;
	//ステージ
	std::shared_ptr<PanelManager> stage;

	BlockAttribute attribute;


	void PlayerTurn();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

	// 現在のステージ
	Stages::StageData m_NowStage;
	// 現在のウェーブ
	int m_NowWave;
	// ステージをセット
	void SetStage(std::string StageName);
	// 次のウェーブスタート
	void NextWave();
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