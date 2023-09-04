#pragma once
#include<memory>
#include"Render/RenderObject/Camera.h"
#include"KuroEngine.h"

namespace KuroEngine
{
	class DebugCamera
	{
		std::shared_ptr<Camera>m_cam;

		//�X�P�[�����O
		Vec2<float>m_scale;
		
		//�����_�̃g�����X�t�H�[���i���W���̂݊i�[�j
		Transform m_gazePointTransform;

		//�����_���ǂ̍��W�p�x���猩�Ă��邩�̏��̃g�����X�t�H�[��
		Transform m_posAngleTransform;

	public:
		DebugCamera();
		void Init(const Vec3<float>& InitPos);
		void Move();

		operator std::shared_ptr<Camera>& ()
		{
			return m_cam;
		}

		operator Camera& ()
		{
			return *m_cam;
		}
	};
}