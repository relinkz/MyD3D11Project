#ifndef ENTITY_H
#define ENTITY_H

#include <DirectXMath.h>
#include <D3D11.h>
#include <string>
#include <vector>
#include <map>

struct VertexData
{
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subdata;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 norm;
};

struct Face
{
	SimpleVertex a;
	SimpleVertex b;
	SimpleVertex c;
};

class Entity
{
private:
	std::vector<DirectX::XMFLOAT3> m_pos;
	std::vector<DirectX::XMFLOAT3> m_norm;
	
	std::vector<std::string> m_faceDesc;
	std::vector<Face> m_faces;

	std::vector<std::string> m_mtlObjects;

	std::map<const std::string, const DirectX::XMMATRIX> m_materials;

	bool readFromFile(const std::string& dir);
	
	std::string handleObjfile(const std::string& dir);
	bool handleMtlfile(const std::string& path);
	void generateFaces();
	SimpleVertex stringToVertex(const std::string& str);

	bool parseFileDataToString(const std::string& dir);
public:
	Entity();
	virtual ~Entity();

	VertexData getVertexDescription();
	static UINT sizeOfVertex();
	UINT getNrOfVertex() const;

	static int indexCount;

	DirectX::XMMATRIX getTransform(const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& translate) const;
};

#endif // !ENTITY_H

