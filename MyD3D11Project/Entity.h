#ifndef ENTITY_H
#define ENTITY_H

#include <DirectXMath.h>
#include <D3D11.h>

struct VertexData
{
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subdata;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

class Entity
{
protected:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMMATRIX m_scale;
	float m_rotAngle;
private:
	static SimpleVertex* vertices;
	static WORD* indices;
public:
	Entity();
	Entity(float x, float y, float z);
	virtual ~Entity();

	static void createSimpleTriangle();

	static VertexData getVertexDescription();
	static VertexData getIndexDescription();
	static UINT sizeOfVertex();

	static int indexCount;

	void update();
	DirectX::XMMATRIX getTransform() const;
};

#endif // !ENTITY_H

