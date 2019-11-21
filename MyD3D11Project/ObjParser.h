#pragma once

#include <string>
#include <vector>
#include <array>

class ObjParser
{
private:
	std::vector<std::array<float, 4>>	geometricVertices_;
	std::vector<std::array<float, 3>>	parameterSpaceVertices_;
	std::vector<std::array<float, 3>>	vertexNormals_;
	std::vector<std::array<float, 2>>	textureCoordinates_;
public:
	ObjParser(const std::string& src);
	ObjParser() = default;
	~ObjParser() = default;

	std::vector<std::array<float, 4>> getGeometricVerticies()		const { return geometricVertices_; }
	std::vector<std::array<float, 3>> getVertexNormals()			const { return vertexNormals_; }
	std::vector<std::array<float, 3>> getParameterSpaceVertices()	const { return parameterSpaceVertices_; }
	std::vector<std::array<float, 2>> getTextureCoordinates()		const { return textureCoordinates_; }
};

