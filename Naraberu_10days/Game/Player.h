#pragma once
#include "UnitBase.h"

#include "../../src/engine/DirectX12/D3D12App.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D.h"
#include "../../src/engine/ForUser/DrawFunc/2D/DrawFunc2D_Mask.h"
#include "../../src/engine/FrameWork/WinApp.h"

class Player :
    public UnitBase
{
private:

	// ‰æ‘œ
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpFrameTex;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_green;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_yellow;
	std::shared_ptr<KuroEngine::TextureBuffer> m_HpTex_red;

	//ƒLƒƒƒ‰‰æ‘œ
	std::shared_ptr<KuroEngine::TextureBuffer> m_CharacterTex;

public:
	Player();

	void OnInitialize()override;
	void OnUpdate()override;
	void OnAlwaysUpdate()override;
	void OnDraw()override;
	void OnImguiDebug()override;
	void OnFinalize()override;

private:
	KuroEngine::Vec2<float> m_Left_Top;
	KuroEngine::Vec2<float> m_Right_Bottom;
	KuroEngine::Vec2<float> m_Center;
public:
	KuroEngine::Vec2<float> Get_Left_Top() { return m_Left_Top; }
	KuroEngine::Vec2<float> Get_Right_Bottom() { return m_Right_Bottom; }
	KuroEngine::Vec2<float> Get_Center() { return m_Center; }
};

