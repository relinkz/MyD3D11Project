#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	m_eye = DirectX::XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f);
	
	m_at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_at = DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(m_at, m_eye));

	m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}


Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	return DirectX::XMMatrixLookAtLH(m_eye, m_at, m_up);
}

void Camera::setEye(float x, float y, float z)
{
	m_eye = DirectX::XMVectorSet(x, y, z, 0.0f);
}

void Camera::rotateCamera(const DirectX::XMFLOAT3& rotVec, float angle)
{
	auto test = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(rotVec.x, rotVec.y, rotVec.z, 0), angle);
	m_eye = DirectX::XMVector3Rotate(m_eye, test);
}
