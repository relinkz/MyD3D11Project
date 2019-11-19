#pragma once

#include <string>
#include <vector>
#include <array>

class ObjParser
{
private:
	std::vector<std::string>			rawFileData_;
	std::vector<std::array<float, 4>>	geometricVertices_;
	std::vector<std::array<float, 3>>	vertexNormals_;
	std::vector<std::array<float, 3>>	parameterSpaceVertices_;
	std::vector<std::array<float, 2>>	textureCoordinates_;
public:
	ObjParser(const std::string& src);
	ObjParser() = default;
	~ObjParser() = default;

	std::vector<std::string> getRawData() const { return rawFileData_; }
};

