#include "stdafx.h"
#include "ObjParser.h"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>

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
		if (row.find(type) != string::npos)
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

static vector<array<float, 4>> toFloat4Vector(const stringContainer& geoVertList)
{
	vector<array<float, 4>> toReturn;
	for (const string& row : geoVertList)
	{
		stringContainer split = splitStringByCharacter(row, ' ');
		if (split.size() != 3 && split.size() != 4)
		{
			string err = __func__;
			err.append(" ObjParser ERROR: geometric vertices not 3 or 4 by row");

			throw(err);
		}

		array<float, 4> asFloat = { stof(split[0]), stof(split[1]), stof(split[2]), 0.0f };
		if (split.size() == 4)
		{
			asFloat[3] = stof(split[3]);
		}

		toReturn.emplace_back(asFloat);
	}

	return toReturn;
}

static vector<array<float, 3>> toFloat3Vector(const stringContainer& vertexNormalsList)
{
	vector<array<float, 3>> toReturn;

	for (const string& row : vertexNormalsList)
	{
		stringContainer split = splitStringByCharacter(row, ' ');
		if (split.size() != 3)
		{
			string err = __func__;
			err.append(" ObjParser ERROR: toFloat3Vector");

			throw(err);
		}

		array<float, 3> asFloat = { stof(split[0]), stof(split[1]), stof(split[2]) };
		toReturn.emplace_back(asFloat);
	}

	return toReturn;
}

static vector<array<float, 2>> toFloat2Vector(const stringContainer& vertexNormalsList)
{
	vector<array<float, 2>> toReturn;

	for (const string& row : vertexNormalsList)
	{
		stringContainer split = splitStringByCharacter(row, ' ');
		if (split.size() != 2)
		{
			string err = __func__;
			err.append(" ObjParser ERROR: toFloat2Vector");

			throw(err);
		}

		array<float, 2> asFloat = { stof(split[0]), stof(split[1]) };
		toReturn.emplace_back(asFloat);
	}

	return toReturn;
}

ObjParser::ObjParser(const string& src)
{
	try
	{
		const stringContainer rawFileData = storeFileAsString(src);

		const stringContainer verts		= fetchAllStringsOfType("v ", rawFileData);
		const stringContainer vTexts	= fetchAllStringsOfType("vt ", rawFileData);
		const stringContainer vNorms	= fetchAllStringsOfType("vn ", rawFileData);

		geometricVertices_				= toFloat4Vector(verts);
		textureCoordinates_				= toFloat2Vector(vTexts);
		vertexNormals_					= toFloat3Vector(vNorms);
	}
	catch (const string msg)
	{
		cout << msg << endl;
		assert(false);
	}
}
