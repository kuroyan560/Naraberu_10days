#pragma once
#include"DirectX12/D3D12Data.h"

namespace KuroEngine
{
	class ScanLine
	{
		static const int THREAD_DIV = 16;

		//��������`���������{���R���s���[�g�p�C�v���C��
		static std::shared_ptr<ComputePipeline>s_pipeline;

		struct ConstData
		{
			//�e�N�X�`���̍���
			int m_texHeight;
			//���g��
			int m_frequency = 10;
		}m_constData;
		//�萔�o�b�t�@
		std::shared_ptr<ConstantBuffer>m_constBuff;

		//�|�X�g�G�t�F�N�g������������
		std::shared_ptr<TextureBuffer>m_resultTex;

	public:
		ScanLine(Vec2<int>arg_texSize, DXGI_FORMAT arg_texFormat, int arg_frequency = 10);
		void SetFrequency(int arg_frequency);

		//�O���t�B�b�N�X�}�l�[�W���ɓo�^
		void Register(const std::shared_ptr<TextureBuffer>& arg_srcTex);
		//���ʂ̃e�N�X�`���擾
		std::shared_ptr<TextureBuffer>& GetResultTex() { return m_resultTex; }
		//�E�B���h�E�T�C�Y�Ō��ʂ�`��
		void DrawResult(const AlphaBlendMode& arg_alphaBlend);
	};
}