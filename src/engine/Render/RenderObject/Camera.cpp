#include "Camera.h"
#include"DirectX12/D3D12App.h"
#include"FrameWork/WinApp.h"

void KuroEngine::Camera::CameraInfoUpdate()
{
	if (dirty || m_transform.IsDirty())
	{
		cameraInfo.matView = XMMatrixInverse(nullptr, m_transform.GetMatWorld());
		//cameraInfo.eye = { cameraInfo.matView.r[3].m128_f32[0],cameraInfo.matView.r[3].m128_f32[1] ,cameraInfo.matView.r[3].m128_f32[2] };
		cameraInfo.eye = m_transform.GetPosWorld();

		DirectX::XMVECTOR cameraAxisX = m_transform.GetRightWorld();
		DirectX::XMVECTOR cameraAxisY= m_transform.GetUpWorld();
		DirectX::XMVECTOR cameraAxisZ = m_transform.GetFrontWorld();

		//�S�����r���{�[�h�s��̌v�Z
		cameraInfo.billboardMat = XMMatrixInverse(nullptr, cameraInfo.matView);
		//cameraInfo.billboardMat.r[0] = cameraAxisX;
		//cameraInfo.billboardMat.r[1] = cameraAxisY;
		//cameraInfo.billboardMat.r[2] = cameraAxisZ;
		cameraInfo.billboardMat.r[3] = XMVectorSet(0, 0, 0, 1);

		//Y�����r���{�[�h�s��̌v�Z
		//�J����X���AY���AZ��
		XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

		//X���͋���
		ybillCameraAxisX = cameraAxisX;
		//Y���̓��[���h���W�n��Y��
		ybillCameraAxisY = XMVector3Normalize(cameraAxisY);
		//Z����X����Y���̊O�ςŋ��܂�
		ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

		//Y�����r���{�[�h�s��
		cameraInfo.billboardMatY.r[0] = ybillCameraAxisX;
		cameraInfo.billboardMatY.r[1] = ybillCameraAxisY;
		cameraInfo.billboardMatY.r[2] = ybillCameraAxisZ;
		cameraInfo.billboardMatY.r[3] = XMVectorSet(0, 0, 0, 1);

		if (projMatMode == Perspective)
		{
			cameraInfo.matProjection = XMMatrixPerspectiveFovLH(
				angleOfView,								//��p
				aspect,	//�A�X�y�N�g��
				nearZ, farZ);		//�O�[�A���[
		}
		else
		{
			cameraInfo.matProjection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
		}
		cameraInfo.nearClip = nearZ;
		cameraInfo.farClip = farZ;

		buff->Mapping(&cameraInfo);

		viewInvMat = XMMatrixInverse(nullptr, cameraInfo.matView);

		dirty = false;
	}
}

KuroEngine::Camera::Camera(const std::string& Name) : name(Name)
{
	auto initData = ConstData();
	buff = D3D12App::Instance()->GenerateConstantBuffer(sizeof(ConstData), 1, &initData, Name.c_str());
	aspect = KuroEngine::WinApp::Instance()->GetAspect();
}

const std::shared_ptr<KuroEngine::ConstantBuffer>& KuroEngine::Camera::GetBuff()
{
	CameraInfoUpdate();
	return buff;
}