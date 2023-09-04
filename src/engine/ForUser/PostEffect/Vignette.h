#pragma once
#include<memory>
#include"DirectX12/D3D12Data.h"
#include"ForUser/Debugger.h"
namespace KuroEngine
{
	class SpriteMesh;

	//�r�l�b�g�|�X�g�G�t�F�N�g�i���ӌ����j
	class Vignette : public Debugger
	{
	public:
		struct Info
		{
			//���ӌ����̐F
			Color m_color = Color(0.0f, 0.0f, 0.0f, 1.0f);
			//���ӌ����̒��S�iUV���W�j
			Vec2<float>m_center = { 0.5f,0.5f };
			//�r�l�b�g��
			float m_intensity = 1.0f;
			//�r�l�b�g�̊��炩��
			float m_smoothness = 0.0f;
		};
	private:
		std::shared_ptr<SpriteMesh>m_spriteMesh;
		std::shared_ptr<ConstantBuffer>m_vignetteInfoBuff;
		std::shared_ptr<RenderTarget>m_resultTex;

		Info m_vignetteInfo;

		void OnImguiItems()override;

	public:
		Vignette();

		//�O���t�B�b�N�X�}�l�[�W���ɓo�^
		void Register(const std::shared_ptr<TextureBuffer>& arg_srcTex);
		//���ʂ̃e�N�X�`���擾
		std::shared_ptr<RenderTarget>& GetResultTex() { return m_resultTex; }
		//�E�B���h�E�T�C�Y�Ō��ʂ�`��
		void DrawResult(const AlphaBlendMode& arg_alphaBlend);

		//���̃Q�b�^�E�Z�b�^
		const Info& GetInfo()const { return m_vignetteInfo; }
		void SetInfo(const Info& arg_info);
	};
}