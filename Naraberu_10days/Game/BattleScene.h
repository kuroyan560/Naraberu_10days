#pragma once
#include "KuroEngineDevice.h"

#include "Player.h" 
#include "Enemy.h"
#include "BattleTurnMgr.h"
#include "BlockManager.h"
#include "StageManager.h"

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
	//�u���b�N
	std::shared_ptr<BlockManager> block;
	//�X�e�[�W
	std::shared_ptr<StageManager> stage;

	BlockAttribute attribute;


	void PlayerTurn();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;
public:
	BattleScene();

};

