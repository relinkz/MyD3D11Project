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

ObjParser::ObjParser(const string& src)
{
	try
	{
		const stringContainer rawFileData = storeFileAsString(src);

		const stringContainer v		= fetchAllStringsOfType("v ", rawFileData);
		const stringContainer vt	= fetchAllStringsOfType("vt ", rawFileData);
		const stringContainer vn	= fetchAllStringsOfType("vn ", rawFileData);
		const stringContainer vp	= fetchAllStringsOfType("vp ", rawFileData);

		geometricVertices_			= toFloat4Vector(v, 1.0f);
		textureCoordinates_			= toFloat3Vector(vt);
		vertexNormals_				= toFloat3Vector(vn);
		parameterSpaceVertices_		= toFloat3Vector(vp);
	}
	catch (const string msg)
	{
		cout << msg << endl;
		assert(false);
	}
}
