#include "Sprite_Shadow.h"
#include"KuroEngineDevice.h"
#include"Render/RenderObject/LightManager.h"

std::shared_ptr<KuroEngine::GraphicsPipeline>KuroEngine::Sprite_Shadow::s_transPipeline;
std::shared_ptr<KuroEngine::ConstantBuffer>KuroEngine::Sprite_Shadow::s_EyePosBuff;
std::shared_ptr<KuroEngine::TextureBuffer>KuroEngine::Sprite_Shadow::s_defaultTex;
std::shared_ptr<KuroEngine::TextureBuffer>KuroEngine::Sprite_Shadow::s_defaultNormalMap;
std::shared_ptr<KuroEngine::TextureBuffer>KuroEngine::Sprite_Shadow::s_defaultEmissiveMap;

void KuroEngine::Sprite_Shadow::SetEyePos(Vec3<float> EyePos)
{
	s_EyePosBuff->Mapping(&EyePos);
}

KuroEngine::Sprite_Shadow::Sprite_Shadow(const std::shared_ptr<TextureBuffer>& Texture, const std::shared_ptr<TextureBuffer>& NormalMap, const std::shared_ptr<TextureBuffer>& EmissiveMap, const char* Name)
{
	//静的メンバの初期化
	if (!s_transPipeline)
	{
		//パイプライン設定
		static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		PIPELINE_OPTION.m_depthTest = false;

		//インプットレイアウト
		static std::vector<InputLayoutParam>INPUT_LAYOUT =
		{
			InputLayoutParam("POSITION",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("TEXCOORD",DXGI_FORMAT_R32G32_FLOAT)
		};

		//レンダーターゲット描画先情報
		static std::vector<RenderTargetInfo>RENDER_TARGET_INFO =
		{
			//バックバッファのフォーマット、アルファブレンド
			RenderTargetInfo(D3D12App::Instance()->GetBackBuffFormat(),AlphaBlendMode_Trans),
			//エミッシブ
			RenderTargetInfo(DXGI_FORMAT_R32G32B32A32_FLOAT,AlphaBlendMode_Trans)
		};

		//シェーダー情報
		static Shaders SHADERS;
		SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/engine/Sprite_Shadow.hlsl", "VSmain", "vs_6_4");
		SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/engine/Sprite_Shadow.hlsl", "PSmain", "ps_6_4");

		//ルートパラメータ
		static std::vector<RootParam>ROOT_PARAMETER =
		{
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"平行投影行列定数バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"カラー & 行列定数 & スプライトのZ設定値バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"視点座標情報"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"アクティブ中のライト数バッファ"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"ディレクションライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"ポイントライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"スポットライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"天球ライト情報 (構造化バッファ)"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"テクスチャシェーダーリソース"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"ノーマルマップシェーダーリソース"),
			RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"エミッシブマップシェーダーリソース"),
		};

		//パイプライン生成
		s_transPipeline = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(false, true) });

		//視点座標
		Vec3<float>INIT_EYE_POS = { KuroEngine::WinApp::Instance()->GetExpandWinCenter().x,KuroEngine::WinApp::Instance()->GetExpandWinCenter().y,-5.0f };
		s_EyePosBuff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(Vec3<float>), 1, &INIT_EYE_POS, "Sprite_Shadow - EYE_POS");

		//白テクスチャ
		s_defaultTex = D3D12App::Instance()->GenerateTextureBuffer(Color(1.0f, 1.0f, 1.0f, 1.0f));

		// (0,0,-1) ノーマルマップ
		s_defaultNormalMap = D3D12App::Instance()->GenerateTextureBuffer(Color(0.5f, 0.5f, 1.0f, 1.0f));

		//黒テクスチャ
		s_defaultEmissiveMap = D3D12App::Instance()->GenerateTextureBuffer(Color(0.0f, 0.0f, 0.0f, 1.0f));
	}

	//デフォルトのテクスチャバッファ
	m_texBuff = s_defaultTex;
	m_normalMap = s_defaultNormalMap;
	m_emissiveMap = s_defaultEmissiveMap;
	
	//テクスチャセット
	SetTexture(Texture, NormalMap, EmissiveMap);

	//行列初期化
	m_constData.m_mat = m_transform.GetMatWorld();

	//定数バッファ生成
	m_constBuff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(m_constData), 1, &m_constData, Name);
}

void KuroEngine::Sprite_Shadow::SetTexture(const std::shared_ptr<TextureBuffer>& Texture, const std::shared_ptr<TextureBuffer>& NormalMap, const std::shared_ptr<TextureBuffer>& EmissiveMap)
{
	if (Texture != nullptr)
	{
		m_texBuff = Texture;
	}
	if (NormalMap != nullptr)
	{
		m_normalMap = NormalMap;
	}
	if (EmissiveMap != nullptr)
	{
		m_emissiveMap = EmissiveMap;
	}
}

void KuroEngine::Sprite_Shadow::SetColor(const Color& Color)
{
	if (m_constData.m_color == Color)return;
	m_constData.m_color = Color;
	m_constBuff->Mapping(&m_constData);
}

void KuroEngine::Sprite_Shadow::SetDepth(const float& Depth)
{
	if (m_constData.z == Depth)return;
	m_constData.z = Depth;
	m_constBuff->Mapping(&m_constData);
}

void KuroEngine::Sprite_Shadow::SetDiffuseAffect(const float& Diffuse)
{
	if (m_constData.m_diffuse == Diffuse)return;
	m_constData.m_diffuse = Diffuse;
	m_constBuff->Mapping(&m_constData);
}

void KuroEngine::Sprite_Shadow::SetSpecularAffect(const float& Specular)
{
	if (m_constData.m_specular == Specular)return;
	m_constData.m_specular = Specular;
	m_constBuff->Mapping(&m_constData);
}

void KuroEngine::Sprite_Shadow::SetLimAffect(const float& Lim)
{
	if (m_constData.m_lim == Lim)return;
	m_constData.m_lim = Lim;
	m_constBuff->Mapping(&m_constData);
}

void KuroEngine::Sprite_Shadow::Draw(LightManager& LigManager)
{
	KuroEngine::KuroEngineDevice::Instance()->Graphics().SetGraphicsPipeline(s_transPipeline);

	if (m_transform.IsDirty())
	{
		m_constData.m_mat = m_transform.GetMatWorld();
		m_constBuff->Mapping(&m_constData);
	}

	m_mesh.Render({
		{KuroEngine::KuroEngineDevice::Instance()->GetParallelMatProjBuff(),CBV},
		{m_constBuff,CBV},
		{s_EyePosBuff,CBV},
		{LigManager.GetLigNumInfo(),CBV},
		{LigManager.GetLigInfo(Light::DIRECTION),SRV},
		{LigManager.GetLigInfo(Light::POINT),SRV},
		{LigManager.GetLigInfo(Light::SPOT),SRV},
		{LigManager.GetLigInfo(Light::HEMISPHERE),SRV},
		{m_texBuff,SRV},
		{m_normalMap,SRV},
		{m_emissiveMap,SRV},
		});
}
