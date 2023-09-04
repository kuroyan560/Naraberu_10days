#pragma once
#include "KuroEngineDevice.h"

#include "Player.h" 
#include "Enemy.h"
#include "BattleTurnMgr.h"

class BattleScene :
    public KuroEngine::BaseScene
{
	// �v���C���[
	std::shared_ptr<Player> Pl;
	// �G���X�g
	std::vector<std::shared_ptr<UnitBase>> En;
	// �o�g��
	BattleTurnMgr Mgr;
	// �w�i
	std::shared_ptr<KuroEngine::TextureBuffer> m_SukasiTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_BackTex;

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	BattleScene();
};

