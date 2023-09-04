#include "DOF.h"
#include"KuroEngineDevice.h"
#include"ForUser/PostEffect/GaussianBlur.h"
#include"ForUser/DrawFunc/2D/DrawFunc2D.h"

std::shared_ptr<KuroEngine::ComputePipeline>KuroEngine::DOF::s_pipeline;

void KuroEngine::DOF::GeneratePipeline()
{
	std::vector<RootParam>rootParam =
	{
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,"DOF�ݒ�"),
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"���̃e�N�X�`��"),
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�u���[���������e�N�X�`��"),
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,"�[�x�}�b�v"),
		RootParam(D3D12_DESCRIPTOR_RANGE_TYPE_UAV,"�`�����ݐ�o�b�t�@")
	};

	auto cs = D3D12App::Instance()->CompileShader("resource/engine/DOF.hlsl", "CSmain", "cs_6_4");
	s_pipeline = D3D12App::Instance()->GenerateComputePipeline(cs, rootParam, { WrappedSampler(false, true) });
}

void KuroEngine::DOF::OnImguiItems()
{
	if (CustomParamDirty())
	{
		m_configBuff->Mapping(&m_config);
		m_gaussianBlur->SetBlurPower(m_blurPower);
	}
}

KuroEngine::DOF::DOF() : Debugger("DOF")
{
	//�p�C�v���C���������Ȃ琶��
	if (!s_pipeline)GeneratePipeline();

	AddCustomParameter("nearPint", { "Config","nearPint" }, PARAM_TYPE::FLOAT, &m_config.m_nearPint, "Config");
	AddCustomParameter("farPint", { "Config","farPint" }, PARAM_TYPE::FLOAT, &m_config.m_farPint, "Config");
	AddCustomParameter("pintLength", { "Config","pintLength" }, PARAM_TYPE::FLOAT, &m_config.m_pintLength, "Config");
	AddCustomParameter("power", { "Config","Blur" }, PARAM_TYPE::FLOAT, &m_blurPower, "Blur");
	LoadParameterLog();

	//�E�B���h�E�T�C�Y�̎擾
	const auto winSize = KuroEngine::WinApp::Instance()->GetExpandWinSize();

	//�o�b�N�o�b�t�@�̎擾
	auto backBuffFormat = D3D12App::Instance()->GetBackBuffFormat();

	//DOF�ݒ�
	m_configBuff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(DOFConfig), 1, &m_config, "DOF - Config - ConstantBuffer");

	//�K�E�V�A���u���[����
	m_gaussianBlur = std::make_shared<GaussianBlur>(winSize.Int(), backBuffFormat, m_blurPower);

	//�[�x�}�b�v�����Ƃɐ����������߃{�P�摜�̊i�[��
	m_processedTex = D3D12App::Instance()->GenerateTextureBuffer(winSize.Int(), backBuffFormat, "DOF - ProcessedTex");


}

void KuroEngine::DOF::SetPintConfig(float NearPint, float FarPint, float PintLength)
{
	m_config.m_nearPint = NearPint;
	m_config.m_farPint = FarPint;
	m_config.m_pintLength = PintLength;
	m_configBuff->Mapping(&m_config);
}

void KuroEngine::DOF::Register(std::weak_ptr<TextureBuffer> Src, std::weak_ptr<TextureBuffer> DepthMap)
{
	m_gaussianBlur->Register(Src.lock());

	KuroEngine::KuroEngineDevice::Instance()->Graphics().SetComputePipeline(s_pipeline);

	static const int DIV = 32;
	Vec3<int>threadNum = 
	{ 
		static_cast<int>(ceil(m_processedTex->GetGraphSize().x / DIV) + 1),
		static_cast<int>(ceil(m_processedTex->GetGraphSize().y / DIV) + 1),
		1 
	};

	KuroEngine::KuroEngineDevice::Instance()->Graphics().Dispatch(threadNum,
		{
			{m_configBuff,CBV},
			{Src.lock(),SRV},
			{m_gaussianBlur->GetResultTex(),SRV},
			{DepthMap.lock(),SRV},
			{m_processedTex,UAV}
		});
}

void KuroEngine::DOF::Draw()
{
	KuroEngine::DrawFunc2D::DrawGraph({ 0,0 }, m_processedTex);
}

void KuroEngine::DOF::SetBlurPower(float arg_blurPower)
{
	m_blurPower = arg_blurPower;
	m_gaussianBlur->SetBlurPower(m_blurPower);
}
