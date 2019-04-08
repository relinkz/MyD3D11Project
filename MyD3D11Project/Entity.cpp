#include "stdafx.h"
#include "Entity.h"

// common data between all entities
int Entity::indexCount = 12;
SimpleVertex* Entity::vertices = nullptr;

WORD* Entity::indices = nullptr;


Entity::Entity() = default;

Entity::Entity(float x, float y, float z) : m_pos(x, y, z)
{
	m_rotAngle = 0.0f;
	m_scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
}


Entity::~Entity()
{
	if (Entity::vertices)
	{
		delete Entity::vertices;
		Entity::vertices = nullptr;
	}

	if (Entity::indices)
	{
		delete Entity::indices;
		Entity::indices = nullptr;
	}
}

void Entity::createSimpleTriangle()
{
	if (!Entity::vertices)
	{
		Entity::vertices = new SimpleVertex[8];

		// front Pos, Color, Normal
		Entity::vertices[0] = { DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) , DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) };
		Entity::vertices[1] = { DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) , DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) };
		Entity::vertices[2] = { DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) };
		Entity::vertices[3] = { DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) , DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) };
		
		Entity::vertices[4] = { DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) , DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) };
		Entity::vertices[5] = { DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) , DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) };
		Entity::vertices[6] = { DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) };
		Entity::vertices[7] = { DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) , DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) };
	}

	if (!Entity::indices)
	{
		Entity::indices = new WORD[Entity::indexCount];

		Entity::indices[0] = 1;
		Entity::indices[1] = 2;
		Entity::indices[2] = 0;

		Entity::indices[3] = 1;
		Entity::indices[4] = 0;
		Entity::indices[5] = 3;

		Entity::indices[6] = 4;		//back
		Entity::indices[7] = 6;
		Entity::indices[8] = 5;

		Entity::indices[9] = 7;
		Entity::indices[10] = 4;
		Entity::indices[11] = 5;
	}
}

VertexData Entity::getVertexDescription()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	VertexData data;
	data.desc = bd;
	data.subdata = initData;

	return data;
}

VertexData Entity::getIndexDescription()
{	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * Entity::indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indices;
	
	VertexData data;
	data.desc = bd;
	data.subdata = initData;

	return data;
}

UINT Entity::sizeOfVertex()
{
	return sizeof(SimpleVertex);
}

void Entity::update()
{
	m_rotAngle += 0.0001f;
}

DirectX::XMMATRIX Entity::getTransform() const
{
	DirectX::XMMATRIX transform = m_scale;

	transform *= DirectX::XMMatrixRotationY(m_rotAngle);
	transform *= DirectX::XMMatrixRotationZ(m_rotAngle);
	transform *= DirectX::XMMatrixRotationX(m_rotAngle);

	transform *= DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	return DirectX::XMMatrixTranspose(transform);
}
