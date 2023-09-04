#pragma once
#include"Common/Color.h"
#include"Common/Vec.h"
#include"ForUser/Debugger.h"
#include"DirectX12/D3D12Data.h"
#include<memory>
namespace KuroEngine
{
	class ComputePipeline;
	class ConstantBuffer;
	class TextureBuffer;

	//�t�H�O�|�X�g�G�t�F�N�g
	class Fog : public Debugger
	{
		static const int THREAD_PER_NUM = 32;

		//�t�H�O�̐ݒ�
		struct Config
		{
			//����
			float m_intensity = 1.0f;
			//�s�����x�ő�
			float m_opacityMax = 1.0f;
			//�t�H�O��������ő�[�x
			float m_distMax = 20.0f;
			//�t�H�O��������ŏ��[�x
			float m_distMin = 0.0f;
		}m_config;
		std::shared_ptr<ConstantBuffer>m_configBuffer;

		//�F�Ɋւ���
		struct ColorParameter
		{
			//�e�N�X�`�����g��Ȃ��ꍇ�̃t�H�O�̐F
			Color m_fogColorNear = Color(1.0f, 1.0f, 1.0f, 1.0f);
			Color m_fogColorFar = Color(1.0f, 1.0f, 1.0f, 1.0f);
			//�e�N�X�`�����g����
			unsigned int m_useTex = 0;
			int m_isGameScene;
			KuroEngine::Vec2<float> m_pad;
		}m_colorConfig;
		std::shared_ptr<ConstantBuffer>m_colorConfigBuffer;

		//�t�H�O��������p�C�v���C��
		std::shared_ptr<ComputePipeline>m_cPipeline;

		//�t�H�O�̐F�����߂�e�N�X�`���i�����̃O���f�[�V�����摜�A�[�x�l���������ق� UV �� U�l ���������Ȃ�j
		std::shared_ptr<TextureBuffer>m_gradationTex;

		//���ʂ̕`���
		std::shared_ptr<TextureBuffer>m_resultTex;

		void OnImguiItems()override;

	public:
		Fog(Vec2<int>arg_size, DXGI_FORMAT arg_mainFormat);

		//�O���t�B�b�N�X�}�l�[�W���ɓo�^
		void Register(const std::shared_ptr<TextureBuffer>& arg_main, const std::shared_ptr<TextureBuffer>& arg_depthMap, std::shared_ptr<TextureBuffer>arg_maskTex = nullptr, bool arg_isGameScene = false);

		//���ʂ̃e�N�X�`���擾
		std::shared_ptr<TextureBuffer>& GetResultTex() { return m_resultTex; }
		//�E�B���h�E�T�C�Y�Ō��ʂ�`��
		void DrawResult(const AlphaBlendMode& arg_alphaBlend);

		//�t�H�O�̐F�����߂�O���f�[�V�����e�N�X�`���i�����j���A�^�b�`
		void AttachGradationTex(std::shared_ptr<TextureBuffer>arg_tex);
	};
}