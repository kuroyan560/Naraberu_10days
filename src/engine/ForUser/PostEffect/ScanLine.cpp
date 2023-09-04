#include "ScanLine.h"
#include"KuroEngineDevice.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"

std::shared_ptr<KuroEngine::ComputePipeline>KuroEngine::ScanLine::s_pipeline;

KuroEngine::ScanLine::ScanLine(Vec2<int> arg_texSize, DXGI_FORMAT arg_texFormat, int arg_frequency)
{
	//�R���s���[�g�p�C�v���C������
	if (s_pipeline)
	{
		auto cs = D3D12App::Instance()->CompileShader(
			"resource/engine/ScanLine.hlsl", "CSmain", "cs_6_4");

		s_pipeline = D3D12App::Instance()->GenerateComputePipeline(
			cs,
			{
				{D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"�������Ɋւ�����萔�o�b�t�@"},
				{D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�\�[�X�ƂȂ�e�N�X�`��"},
				{D3D12_DESCRIPTOR_RANGE_TYPE_UAV,"���H��̃e�N�X�`��"},
			},
			{ WrappedSampler(false,true) }
		);
	}

	m_constData.m_texHeight = arg_texSize.y;
	SetFrequency(arg_frequency);

	m_resultTex = D3D12App::Instance()->GenerateTextureBuffer(
		arg_texSize,
		arg_texFormat,
		"ScanLine - ResultTex");
}

void KuroEngine::ScanLine::SetFrequency(int arg_frequency)
{
	m_constData.m_frequency = arg_frequency;
	m_constBuff->Mapping(&m_constData);
}

void KuroEngine::ScanLine::Register(const std::shared_ptr<TextureBuffer>& arg_srcTex)
{
	auto texSize = arg_srcTex->GetGraphSize();

	KuroEngine::KuroEngineDevice::Instance()->Graphics().SetComputePipeline(s_pipeline);
	KuroEngine::KuroEngineDevice::Instance()->Graphics().Dispatch(
		{ texSize.x / THREAD_DIV + 1,texSize.y + THREAD_DIV + 1,1 },
		{
			{m_constBuff,CBV},
			{arg_srcTex,SRV},
			{m_resultTex,UAV},
		});
}

void KuroEngine::ScanLine::DrawResult(const AlphaBlendMode& arg_alphaBlend)
{
	KuroEngine::DrawFunc2D::DrawExtendGraph2D({ 0,0 }, KuroEngine::WinApp::Instance()->GetExpandWinSize(), m_resultTex, 1.0f, arg_alphaBlend);
}
