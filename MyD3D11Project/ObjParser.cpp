#include "stdafx.h"
#include "ObjParser.h"
#include <fstream>
#include <iostream>

static std::vector<std::string> storeFileAsString(const std::string& src)
{
	std::ifstream modelfile(src.c_str());
	std::vector<std::string> toReturn;

	if (!modelfile.is_open())
	{
		throw("Error: Fail to open file: " + src);
	}

	std::string line = "";
	while (std::getline(modelfile, line))
	{
		toReturn.emplace_back(line);
	}
	return toReturn;
}

static std::vector<std::string> fetchAllStringsOfType(const std::string& type, const std::vector<std::string>& data)
{
	std::vector<std::string> toReturn;

	for (const std::string& row : data)
	{
		if (row.find(type) != std::string::npos)
		{
			toReturn.emplace_back(row.substr(type.size(), row.size()));
		}
	}

	return toReturn;
}

ObjParser::ObjParser(const std::string& src)
{
	try
	{
		rawFileData_ = storeFileAsString(src);
	}
	catch (const std::string msg)
	{
		std::cout << msg << std::endl;
	}
}
