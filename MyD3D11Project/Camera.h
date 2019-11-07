#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
#include <D3D11.h>

class Camera
{
private:
	DirectX::XMVECTOR m_eye;
	DirectX::XMVECTOR m_at;
	DirectX::XMVECTOR m_up;

public:
	Camera();
	virtual ~Camera();

	DirectX::XMMATRIX getViewMatrix() const;

	void setEye(float x, float y, float z);
	void rotateCamera(const DirectX::XMFLOAT3& rotVec, float angle);
};

#endif