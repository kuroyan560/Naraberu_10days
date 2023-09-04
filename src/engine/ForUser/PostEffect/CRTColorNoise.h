#pragma once
#include"Common/Color.h"
#include"DirectX12/D3D12Data.h"
#include<map>
#include"ForUser/Debugger.h"

namespace KuroEngine
{
	class SpriteMesh;

	class CRTColorNoise : public Debugger
	{
		struct Info
		{
			//��ʃT�C�Y
			Vec2<float>m_screenSize;
			//��ʘc�݋���
			float m_screenDistort = 0.1f;
			//�K�E�V�A���t�B���^�������ăT���v�����O����ۂ̏d��
			float gaussianSampleWeight = 16.0f;
			//�I�[�o�[���C�F
			Color m_overlayColor;
			//�I�[�o�[���C�̖��邳�������l
			float m_overlayThreshold = 0.5f;
			//�ʓx���グ
			float m_saturationRate = 1.0f;
			float m_pad[2];
		}m_crtInfo;

		std::shared_ptr<SpriteMesh>m_spriteMesh;
		std::shared_ptr<ConstantBuffer>m_crtInfoBuff;
		std::shared_ptr<RenderTarget>m_resultTex;

		void OnImguiItems()override;

	public:
		CRTColorNoise();

		//�O���t�B�b�N�X�}�l�[�W���ɓo�^
		void Register(const std::shared_ptr<TextureBuffer>& arg_srcTex);
		//���ʂ̃e�N�X�`���擾
		std::shared_ptr<RenderTarget>& GetResultTex() { return m_resultTex; }
		//�E�B���h�E�T�C�Y�Ō��ʂ�`��
		void DrawResult(const AlphaBlendMode& arg_alphaBlend);
	};
}