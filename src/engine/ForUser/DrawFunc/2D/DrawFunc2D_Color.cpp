#include "DrawFunc2D_Color.h"
#include"KuroEngineDevice.h"

//各塗り方のエントリポイント名
std::array<std::string, static_cast<int>(KuroEngine::DrawFunc2D_Color::FILL_MDOE::NUM)> KuroEngine::DrawFunc2D_Color::s_fillModeEntoryPoint =
{
	"PSmain_None",
	"PSmain_Mul",
};

//DrawExtendGraph
int KuroEngine::DrawFunc2D_Color::s_DrawExtendGraphCount = 0;

//DrawRotaGraph
int KuroEngine::DrawFunc2D_Color::s_DrawRotaGraphCount = 0;

static std::vector<KuroEngine::RootParam>ROOT_PARAMETER =
{
	{D3D12_DESCRIPTOR_RANGE_TYPE_CBV, "平行投影行列定数バッファ"},
	{D3D12_DESCRIPTOR_RANGE_TYPE_SRV, "テクスチャリソース"},
};

void KuroEngine::DrawFunc2D_Color::DrawGraph(const Vec2<float>& LeftUpPos, const std::shared_ptr<TextureBuffer>& Tex, const Color& Paint, const Vec2<bool>& Miror, const Vec2<float>& LeftUpPaintUV, const Vec2<float>& RightBottomPaintUV)
{
	DrawExtendGraph2D(LeftUpPos, LeftUpPos + Tex->GetGraphSize().Float(), Tex, Paint, Miror, LeftUpPaintUV, RightBottomPaintUV);
}

void KuroEngine::DrawFunc2D_Color::DrawExtendGraph2D(const Vec2<float>& LeftUpPos, const Vec2<float>& RightBottomPos, const std::shared_ptr<TextureBuffer>& Tex, const Color& Paint, const Vec2<bool>& Miror,
	const Vec2<float>& LeftUpPaintUV, const Vec2<float>& RightBottomPaintUV, const FILL_MDOE& FillMode)
{
	if (RightBottomPaintUV.x < LeftUpPaintUV.x)assert(0);
	if (RightBottomPaintUV.y < LeftUpPaintUV.y)assert(0);

	class ExtendGraphVertex
	{
	public:
		Vec2<float>leftUpPos;
		Vec2<float>rightBottomPos;
		Color paintColor;
		Vec2<int> miror;
		Vec2<float>leftUpPaintUV;
		Vec2<float>rightBottomPaintUV;
		ExtendGraphVertex(const Vec2<float>& LeftUpPos, const Vec2<float>& RightBottomPos, const Color& Paint, const Vec2<bool>& Miror, const Vec2<float>& LeftUpPaintUV, const Vec2<float>& RightBottomPaintUV)
			:leftUpPos(LeftUpPos), rightBottomPos(RightBottomPos), paintColor(Paint), miror({ Miror.x ? 1 : 0 ,Miror.y ? 1 : 0 }), leftUpPaintUV(LeftUpPaintUV), rightBottomPaintUV(RightBottomPaintUV) {}
	};

	static std::array<std::shared_ptr<GraphicsPipeline>, static_cast<int>(FILL_MDOE::NUM)>EXTEND_GRAPH_PIPELINE;
	static std::vector<std::shared_ptr<VertexBuffer>>EXTEND_GRAPH_VERTEX_BUFF;

	//パイプライン未生成
	int fillModeIdx = static_cast<int>(FillMode);
	if (!EXTEND_GRAPH_PIPELINE[fillModeIdx])
	{
		//パイプライン設定
		static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		PIPELINE_OPTION.m_depthTest = false;

		//シェーダー情報
		static Shaders SHADERS;
		SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/engine/DrawExtendGraph_Color.hlsl", "VSmain", "vs_6_4");
		SHADERS.m_gs = D3D12App::Instance()->CompileShader("resource/engine/DrawExtendGraph_Color.hlsl", "GSmain", "gs_6_4");
		SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/engine/DrawExtendGraph_Color.hlsl", s_fillModeEntoryPoint[fillModeIdx], "ps_6_4");

		//インプットレイアウト
		static std::vector<InputLayoutParam>INPUT_LAYOUT =
		{
			InputLayoutParam("POSITION_L_U",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("POSITION_R_B",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("PAINT_COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT),
			InputLayoutParam("MIROR",DXGI_FORMAT_R32G32_SINT),
			InputLayoutParam("PAINT_UV_L_U",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("PAINT_UV_R_B",DXGI_FORMAT_R32G32_FLOAT)
		};

		//レンダーターゲット描画先情報
		std::vector<RenderTargetInfo>RENDER_TARGET_INFO = { RenderTargetInfo(D3D12App::Instance()->GetBackBuffFormat(), AlphaBlendMode_Trans) };
		//パイプライン生成
		EXTEND_GRAPH_PIPELINE[fillModeIdx] = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, {WrappedSampler(true, false)});
	}

	KuroEngine::KuroEngineDevice::Instance()->Graphics().SetGraphicsPipeline(EXTEND_GRAPH_PIPELINE[fillModeIdx]);

	if (EXTEND_GRAPH_VERTEX_BUFF.size() < (s_DrawExtendGraphCount + 1))
	{
		EXTEND_GRAPH_VERTEX_BUFF.emplace_back(D3D12App::Instance()->GenerateVertexBuffer(sizeof(ExtendGraphVertex), 1, nullptr, ("DrawExtendGraph_Color -" + std::to_string(s_DrawExtendGraphCount)).c_str()));
	}

	ExtendGraphVertex vertex(LeftUpPos, RightBottomPos, Paint, Miror, LeftUpPaintUV, RightBottomPaintUV);
	EXTEND_GRAPH_VERTEX_BUFF[s_DrawExtendGraphCount]->Mapping(&vertex);

	KuroEngine::KuroEngineDevice::Instance()->Graphics().ObjectRender(EXTEND_GRAPH_VERTEX_BUFF[s_DrawExtendGraphCount],
		{
			{KuroEngine::KuroEngineDevice::Instance()->GetParallelMatProjBuff(),CBV},
			{Tex,SRV }
		}, 0, true);

	s_DrawExtendGraphCount++;
}

void KuroEngine::DrawFunc2D_Color::DrawRotaGraph2D(const Vec2<float>& Center, const Vec2<float>& ExtRate, const float& Radian, const std::shared_ptr<TextureBuffer>& Tex, const Color& Paint, const Vec2<float>& RotaCenterUV, const Vec2<bool>& Miror,
	const Vec2<float>& LeftUpPaintUV, const Vec2<float>& RightBottomPaintUV)
{
	static std::shared_ptr<GraphicsPipeline>ROTA_GRAPH_PIPELINE;
	static std::vector<std::shared_ptr<VertexBuffer>>ROTA_GRAPH_VERTEX_BUFF;

	if (RightBottomPaintUV.x < LeftUpPaintUV.x)assert(0);
	if (RightBottomPaintUV.y < LeftUpPaintUV.y)assert(0);

	//DrawRotaGraph専用頂点
	class RotaGraphVertex
	{
	public:
		Vec2<float>center;
		Vec2<float> extRate;
		float radian;
		Color paintColor;
		Vec2<float>rotaCenterUV;
		Vec2<int> miror;
		Vec2<float>leftUpPaintUV;
		Vec2<float>rightBottomPaintUV;
		RotaGraphVertex(const Vec2<float>& Center, const Vec2<float>& ExtRate, const float& Radian,
			const Color& Paint, const Vec2<float>& RotaCenterUV, const Vec2<bool>& Miror,
			const Vec2<float>& LeftUpPaintUV, const Vec2<float>& RightBottomPaintUV)
			:center(Center), extRate(ExtRate), radian(Radian), rotaCenterUV(RotaCenterUV), paintColor(Paint), miror({ Miror.x ? 1 : 0,Miror.y ? 1 : 0 }),
			leftUpPaintUV(LeftUpPaintUV), rightBottomPaintUV(RightBottomPaintUV) {}
	};

	//パイプライン未生成
	if (!ROTA_GRAPH_PIPELINE)
	{
		//パイプライン設定
		static PipelineInitializeOption PIPELINE_OPTION(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		PIPELINE_OPTION.m_depthTest = false;

		//シェーダー情報
		static Shaders SHADERS;
		SHADERS.m_vs = D3D12App::Instance()->CompileShader("resource/engine/DrawRotaGraph_Color.hlsl", "VSmain", "vs_6_4");
		SHADERS.m_gs = D3D12App::Instance()->CompileShader("resource/engine/DrawRotaGraph_Color.hlsl", "GSmain", "gs_6_4");
		SHADERS.m_ps = D3D12App::Instance()->CompileShader("resource/engine/DrawRotaGraph_Color.hlsl", "PSmain", "ps_6_4");

		//インプットレイアウト
		static std::vector<InputLayoutParam>INPUT_LAYOUT =
		{
			InputLayoutParam("CENTER",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("EXT_RATE",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("RADIAN",DXGI_FORMAT_R32_FLOAT),
			InputLayoutParam("PAINT_COLOR",DXGI_FORMAT_R32G32B32A32_FLOAT),
			InputLayoutParam("ROTA_CENTER_UV",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("MIROR",DXGI_FORMAT_R32G32_SINT),
			InputLayoutParam("PAINT_UV_L_U",DXGI_FORMAT_R32G32_FLOAT),
			InputLayoutParam("PAINT_UV_R_B",DXGI_FORMAT_R32G32_FLOAT)
		};

		//レンダーターゲット描画先情報
		std::vector<RenderTargetInfo>RENDER_TARGET_INFO = { RenderTargetInfo(D3D12App::Instance()->GetBackBuffFormat(), AlphaBlendMode_Trans) };
		//パイプライン生成
		ROTA_GRAPH_PIPELINE = D3D12App::Instance()->GenerateGraphicsPipeline(PIPELINE_OPTION, SHADERS, INPUT_LAYOUT, ROOT_PARAMETER, RENDER_TARGET_INFO, { WrappedSampler(true, false) });
	}

	KuroEngine::KuroEngineDevice::Instance()->Graphics().SetGraphicsPipeline(ROTA_GRAPH_PIPELINE);

	if (ROTA_GRAPH_VERTEX_BUFF.size() < (s_DrawRotaGraphCount + 1))
	{
		ROTA_GRAPH_VERTEX_BUFF.emplace_back(D3D12App::Instance()->GenerateVertexBuffer(sizeof(RotaGraphVertex), 1, nullptr, ("DrawRotaGraph_Color -" + std::to_string(s_DrawRotaGraphCount)).c_str()));
	}

	RotaGraphVertex vertex(Center, ExtRate, Radian, Paint, RotaCenterUV, Miror, LeftUpPaintUV, RightBottomPaintUV);
	ROTA_GRAPH_VERTEX_BUFF[s_DrawRotaGraphCount]->Mapping(&vertex);

	KuroEngine::KuroEngineDevice::Instance()->Graphics().ObjectRender(ROTA_GRAPH_VERTEX_BUFF[s_DrawRotaGraphCount],
		{
			{KuroEngine::KuroEngineDevice::Instance()->GetParallelMatProjBuff(),CBV},
			{Tex,SRV },
		}, 0, true);

	s_DrawRotaGraphCount++;
}

void KuroEngine::DrawFunc2D_Color::DrawLine2DGraph(const Vec2<float>& FromPos, const Vec2<float>& ToPos, const std::shared_ptr<TextureBuffer>& Tex, const Color& Paint, const int& Thickness, const Vec2<bool>& Mirror)
{
	float distance = FromPos.Distance(ToPos);
	Vec2<float> vec = (ToPos - FromPos).GetNormal();

	auto graphSize = Tex->GetGraphSize().Float();
	Vec2<float>expRate = { distance / graphSize.x,Thickness / graphSize.y };
	Vec2<float>centerPos = FromPos + vec * distance / 2;

	DrawRotaGraph2D(centerPos, expRate, KuroEngine::Math::GetAngle(vec), Tex, Paint, { 0.5f,0.5f }, Mirror);
}
