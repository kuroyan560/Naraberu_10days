#include "Player.h"
#include "../../src/engine/FrameWork/UsersInput.h"


Player::Player()
{
	// ƒ^[ƒ“ŠÖ˜A•Ï”‚Ì‰Šú‰»
	m_isMyTurn = false;
	m_NextTurn = false;
}

void Player::OnInitialize()
{
}

void Player::OnUpdate()
{
	using namespace KuroEngine;
	if (UsersInput::Instance()->KeyOnTrigger(DIK_SPACE)) {
		EndTurn();
	}
}

void Player::OnDraw()
{
}

void Player::OnImguiDebug()
{
}

void Player::OnFinalize()
{
}
