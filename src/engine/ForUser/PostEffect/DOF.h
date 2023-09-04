#pragma once
#include<memory>
#include"ForUser/Debugger.h"

namespace KuroEngine
{
	class RenderTarget;
	class ComputePipeline;
	class ConstantBuffer;
	class GaussianBlur;
	class TextureBuffer;

	//DOF�ݒ���
	struct DOFConfig
	{
		//��O
		float m_nearPint = 0.0f;
		//���i��O�`���܂Ń{�P�Ȃ��j
		float m_farPint = 10.0f;
		//��O�ȍ~�A���ȍ~����{�P�ő�ɂȂ�܂ł̋���
		float m_pintLength = 20.0f;
	};

	class DOF : public Debugger
	{
	private:
		static std::shared_ptr<ComputePipeline>s_pipeline;
		void GeneratePipeline();

	private:
		//DOF�ݒ���
		DOFConfig m_config;
		std::shared_ptr<ConstantBuffer>m_configBuff;

		//�K�E�V�A���u���[
		std::shared_ptr<GaussianBlur>m_gaussianBlur;

		//�[�x�}�b�v�����Ƃɐ����������߃{�P�摜�̊i�[��
		std::shared_ptr<TextureBuffer>m_processedTex;

		//�u���[����
		float m_blurPower = 8.0f;

		void OnImguiItems()override;

	public:
		DOF();
		void SetPintConfig(float NearPint = 0.0f, float FarPint = 10.0f, float PintLength = 20.0f);

		//�O���t�B�b�N�X�}�l�[�W���ɓo�^
		void Register(std::weak_ptr<TextureBuffer>Src, std::weak_ptr<TextureBuffer>DepthMap);

		//���ʂ̃e�N�X�`���擾
		std::shared_ptr<TextureBuffer>& GetResultTex() { return m_processedTex; }

		//���ʂ̕`��
		void Draw();

		//�{�P��ݒ�
		void SetBlurPower(float arg_blurPower);
	};
}