#include "stdafx.h"
#include "Entity.h"
#include "ObjParser.h"
#include <fstream>

static DirectX::XMFLOAT3 stringVector(const std::string& data)
{
	// "1.0000 1.0000 0.0000"
	std::string unit[3];
	std::string rest = data;

	for (size_t i = 0; i < 3; i++)
	{
		if (i == 2)
		{
			unit[i] = rest;
		}
		else
		{
			auto hit = rest.find(" ");
			unit[i] = rest.substr(0, hit + 1);
			rest.erase(0, hit + 1);
		}
	}

	DirectX::XMFLOAT3 toReturn(
		stof(unit[0]),
		stof(unit[1]),
		stof(unit[2]));

	return toReturn;
}

static std::string stringPick(const std::string& line)
{
	std::string substr = line;

	substr.erase(0, substr.find(" ") + 1);

	return substr;
}

void Entity::handleObjfile(const std::string& path)
{
	std::ifstream modelfile(path.c_str());


	// todo Exception?
	if (!modelfile.is_open())
	{
		return;
	}

	std::vector<std::string>stringData;
	while (!modelfile.eof())
	{
		std::string data = "";
		std::getline(modelfile, data);

		stringData.push_back(data);
	}

	m_pos.reserve(stringData.size());
	m_norm.reserve(stringData.size());

	std::string type = "";
	std::string mltFile = "";

	for (size_t i = 0; i < stringData.size(); i++)
	{
		type = stringData[i].substr(0, stringData[i].find(" "));
		stringData[i].erase(0, stringData[i].find(" ") + 1);

		if (type == "f")
		{
			m_faceDesc.push_back(stringData[i]);
		}
		else if(type == "v")
			m_pos.push_back(stringVector(stringData[i]));
		else if(type == "vn")
			m_norm.push_back(stringVector(stringData[i]));
		else if (type == "mtllib")
		{
			mltFile = stringData[i];
		}
		else if (type == "o")
		{
			m_mtlObjects.push_back(stringData[i]);
		}
		else if (type == "#" || type == "")
		{
			// comment
		}
	}

	if (mltFile != "")
	{
		std::string mtlPath = path;
		auto pos = mtlPath.find_last_of('/');
		mtlPath = mtlPath.substr(0, pos + 1);

		mtlPath += mltFile;

		parseFileDataToString(mtlPath);
	}
}

void Entity::handleMtlfile(const std::string& path)
{
	std::ifstream modelfile(path.c_str());

	//todo: exception?
	if (!modelfile.is_open())
	{
		return;
	}

	std::vector<std::string>stringData;
	while (!modelfile.eof())
	{
		std::string data = "";
		std::getline(modelfile, data);

		stringData.push_back(data);
	}

	for (size_t i = 0; i < stringData.size(); i++)
	{
		std::string type = stringData[i].substr(0, stringData[i].find(" "));
		stringData[i].erase(0, stringData[i].find(" ") + 1);

		if (type == "newmtl")
		{
			std::string key = stringData[i];

			auto v1 = stringVector(stringPick(stringData[i + 1]));
			auto v2 = stringVector(stringPick(stringData[i + 2]));
			auto v3 = stringVector(stringPick(stringData[i + 3]));

			DirectX::XMMATRIX mat;
		
			mat.r[0] = DirectX::XMVectorSet(v1.x, v1.y, v1.z, 1);
			mat.r[1] = DirectX::XMVectorSet(v2.x, v2.y, v2.z, 1);
			mat.r[2] = DirectX::XMVectorSet(v3.x, v3.y, v3.z, 1);
			mat.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1);

			m_materials.insert({ key, mat });
		}
	}
}

SimpleVertex Entity::stringToVertex(const std::string& str)
{
	std::string temp;
	std::string rest = str;
	SimpleVertex toReturn;
	
	int vIndex = 0;
	int textIndex = 0;
	int normIndex = 0;
	// v
	auto index = str.find("v");
	if (index != std::string::npos)
	{
		temp = str.substr(index + 1, str.find('/'));
	}
	else
	{
		temp = str.substr(0, str.find('/'));
	}

	rest.erase(0, rest.find('/') + 1);

	vIndex = std::stoi(temp);

	// vt
	index = rest.find("vt");
	if (index != std::string::npos)
	{
		temp = rest.substr(index + 2, rest.find('/') - 1);
	}
	else
	{
		temp = rest.substr(0, rest.find('/'));
	}
	rest.erase(0, rest.find('/') + 1);
	
	textIndex = std::stoi(temp);
	std::string vertexColor = m_mtlObjects.at(textIndex - 1);

	// vn
	index = str.find("vn");
	if (index != std::string::npos)
	{
		rest.erase(0, 2);
	}
	normIndex = std::stoi(rest);

	toReturn.pos = m_pos[vIndex - 1];
	DirectX::XMStoreFloat4(&toReturn.color ,m_materials.find(vertexColor)->second.r[1]);
	toReturn.norm = m_norm[normIndex - 1];

	return toReturn;
}

void Entity::generateFaces()
{
	for (auto it = m_faceDesc.begin(); it != m_faceDesc.end(); it++)
	{

		std::string sVert[3];
		SimpleVertex vertex[3];
		for (int i = 0; i < 3; i++)
		{
			sVert[i] = it->substr(0, it->find(' '));
			vertex[i] = stringToVertex(sVert[i]);
			it->erase(0, it->find(' ')+1);
		}
		Face f;

		f.a = vertex[0];
		f.b = vertex[1];
		f.c = vertex[2];


		m_faces.push_back(f);
	}
}

bool Entity::parseFileDataToString(const std::string& dir)
{
	std::string fileType = dir;
	auto pos = fileType.find_last_of('.');
	fileType.erase(0, pos + 1);

	std::vector<std::string> mtlData;

	if (fileType == "mtl")
	{
		handleMtlfile(dir);
	}
	else if (fileType == "obj")
	{
		handleObjfile(dir);
	}

	return true;
}

bool Entity::readFromFile(const std::string& dir)
{
	parseFileDataToString(dir);

	generateFaces();

	return true;
}

Entity::Entity()
{
	readFromFile("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11Project/cube.obj");
}

Entity::Entity(const std::string & dir)
{
	readFromFile(dir);
}

Entity::~Entity()
{
}

VertexData Entity::getVertexDescription()
{

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Face) * m_faces.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = m_faces.data();
	VertexData data;
	data.desc = bd;
	data.subdata = initData;

	return data;
}

UINT Entity::sizeOfVertex()
{
	return sizeof(SimpleVertex);
}

static DirectX::XMMATRIX applyRotation(const DirectX::XMFLOAT3& angle)
{
	DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixRotationX(angle.x);
	rotMatrix *= DirectX::XMMatrixRotationY(angle.y);
	rotMatrix *= DirectX::XMMatrixRotationZ(angle.z);

	return rotMatrix;
}

DirectX::XMMATRIX Entity::getTransform(const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& angle, const DirectX::XMFLOAT3& translate) const
{
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX rotationMatrix = applyRotation(angle);
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(translate.x, translate.y, translate.z);

	return (scaleMatrix * rotationMatrix * translationMatrix);
}

size_t Entity::getNrOfVertices() const
{
	return m_pos.size();
}

size_t Entity::getNrOfNormals() const
{
	return m_norm.size();
}

UINT Entity::getNrOfFaces() const
{
	return m_faceDesc.size();
}

size_t Entity::getNrOfMtlObj() const
{
	return m_mtlObjects.size();
}
