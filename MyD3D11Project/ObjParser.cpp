#include "stdafx.h"
#include "ObjParser.h"
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

ObjParser::ObjParser(const string& src)
{
	try
	{
		rawFileData_ = storeFileAsString(src);
	}
	catch (const string msg)
	{
		cout << msg << endl;
	}
}
