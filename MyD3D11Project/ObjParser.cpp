#include "stdafx.h"
#include "ObjParser.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std;
using stringContainer = vector<string>;

static stringContainer storeFileAsString(const string& src)
{
	ifstream modelfile(src.c_str());
	stringContainer toReturn;

	if (!modelfile.is_open())
	{
		throw("Error: Fail to open file: " + src);
	}

	string line = "";
	while (getline(modelfile, line))
	{
		toReturn.emplace_back(line);
	}
	return toReturn;
}

static stringContainer fetchAllStringsOfType(const string& type, const stringContainer& data)
{
	stringContainer toReturn;

	for (const string& row : data)
	{
		if ( row[0] != '#' && row.find(type) != string::npos)
		{
			toReturn.emplace_back(row.substr(type.size(), row.size()));
		}
	}

	return toReturn;
}

static stringContainer splitStringByCharacter(const string& str, char c)
{
	stringContainer toReturn;
	string s;

	istringstream stream(str);
	while (getline(stream, s, c))
	{
		toReturn.emplace_back(s);
	}

	return toReturn;
}

// non existing optionals are defaulted as defaultValue, Default= 0.0
static vector<array<float, 4>> toFloat4Vector(const stringContainer& stringList, float defaultValue = 0.0f)
{
	vector<array<float, 4>> toReturn;

	for (const string& row : stringList)
	{
		array<float, 4> asFloat = { defaultValue, defaultValue, defaultValue, defaultValue };
		toReturn.emplace_back(asFloat);

		stringContainer split = splitStringByCharacter(row, ' ');
		for (size_t i = 0; i < split.size(); i++)
		{
			toReturn.back()[i] = stof(split[i]);
		}
	}
	return toReturn;
}

// non existing optionals are defaulted as defaultValue, Default= 0.0
static vector<array<float, 3>> toFloat3Vector(const stringContainer& stringList, float defaultValue = 0.0f)
{
	vector<array<float, 3>> toReturn;

	for (const string& row : stringList)
	{
		array<float, 3> asFloat = { defaultValue, defaultValue, defaultValue };
		toReturn.emplace_back(asFloat);

		stringContainer split = splitStringByCharacter(row, ' ');
		for (size_t i = 0; i < split.size(); i++)
		{
			toReturn.back()[i] = stof(split[i]);
		}
	}
	return toReturn;
}

FaceElement ObjParser::getFaceVertex(const std::string& desc) const
{
	FaceElement element;

	element.v_	= DirectX::XMFLOAT4(0, 0, 0, 0);
	element.vt_ = DirectX::XMFLOAT4(0, 0, 0, 0);
	element.vn_ = DirectX::XMFLOAT4(0, 0, 0, 0);

	// I needto support 1//1
	stringContainer indexes = splitStringByCharacter(desc, '/');
	
	// obj is starting from 1, so I need to offset that
	size_t v =	stoi(indexes[0]) - 1;
	size_t vt = stoi(indexes[1]) - 1;
	size_t vn = stoi(indexes[2]) - 1;

	element.v_	= DirectX::XMFLOAT4(geometricVertices_[v][0], geometricVertices_[v][1], geometricVertices_[v][2], geometricVertices_[v][3]);
	element.vt_ = DirectX::XMFLOAT4(textureCoordinates_[vt][0], textureCoordinates_[vt][1], textureCoordinates_[vt][2], 0.0f);
	element.vn_ = DirectX::XMFLOAT4(vertexNormals_[vn][0], vertexNormals_[vn][1], vertexNormals_[vn][2], 0.0f);

	return element;
}

void ObjParser::generateFaceElements(const std::vector<std::string>& faceList)
{
	for (const string& face : faceList)
	{
		stringContainer faceElem = splitStringByCharacter(face, ' ');
		for (const string& item : faceElem)
			faceElements_.emplace_back(getFaceVertex(item));
	}
}

static std::vector<stringContainer> splitByMaterials(const stringContainer& rawFile)
{
	stringContainer materials = fetchAllStringsOfType("newmtl ", rawFile);
	vector<stringContainer> toReturn;

	size_t walker = 0;
	size_t processingMaterial = 0;

	while (walker < rawFile.size())
	{
		if (rawFile[walker].find(materials[processingMaterial]) != string::npos)
		{
			toReturn.push_back(stringContainer());
			toReturn.back().push_back(rawFile[walker]);
			if (processingMaterial < materials.size() - 1)
			{
				processingMaterial++;
			}
		}
		else if (!toReturn.empty())
		{
			toReturn.back().push_back(rawFile[walker]);
		}
		walker++;
	}

	return toReturn;
}

static Material stringToMaterial(const stringContainer& rawString)
{
	Material toReturn;

	stringContainer holder = fetchAllStringsOfType("illum ", rawString);
	if (!holder.empty())
	{
		//toReturn.illuminationModel_ = IlluminationModel(stoi(holder.front())); // this should be cast to unknown
	}

	return toReturn;
}

void ObjParser::readMtlLibrary(const string& libPath) const
{
	stringContainer rawFileDataMtl;

	try 
	{ 
		rawFileDataMtl = storeFileAsString(libPath); 
	}
	catch (const string msg) 
	{
		cout << msg << endl;
		assert(false);
	}

	vector<stringContainer> splitted = splitByMaterials(rawFileDataMtl);
	stringToMaterial(splitted.front());
}

ObjParser::ObjParser(const string& src)
{
	try
	{
		const stringContainer rawFileData = storeFileAsString(src);

		const stringContainer v		= fetchAllStringsOfType("v ",	rawFileData);
		geometricVertices_			= toFloat4Vector(v, 1.0f);
		
		const stringContainer vt	= fetchAllStringsOfType("vt ",	rawFileData);
		textureCoordinates_			= toFloat3Vector(vt);
		
		const stringContainer vn	= fetchAllStringsOfType("vn ",	rawFileData);
		vertexNormals_				= toFloat3Vector(vn);
		
		const stringContainer vp	= fetchAllStringsOfType("vp ",	rawFileData);
		parameterSpaceVertices_		= toFloat3Vector(vp);
		
		const stringContainer f		= fetchAllStringsOfType("f ",	rawFileData);
		nrOfFaces_ = f.size();
		generateFaceElements(f);

		mtlFiles_					= fetchAllStringsOfType("mtllib ", rawFileData);
		mtlNames_					= fetchAllStringsOfType("usemtl ", rawFileData);
		mtlGroupNames_				= fetchAllStringsOfType("g ", rawFileData);
		mtlSmoothShading_			= fetchAllStringsOfType("s ", rawFileData);

		/*for (const std::string& mtl : mtlFiles_)
		{
			size_t slicePos = src.find_last_of('/');
			const std::string path = src.substr(0, slicePos) + '/' + mtl;

			readMtlLibrary(path);
		}*/
	}
	catch (const string msg)
	{
		cout << msg << endl;
		assert(false);
	}
}

ObjParser::ObjParser()
{
}

ObjParser::~ObjParser()
{
}
