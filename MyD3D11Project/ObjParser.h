#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>
#include <array>

struct FaceElement
{
	DirectX::XMFLOAT4 v_;
	DirectX::XMFLOAT4 vt_;
	DirectX::XMFLOAT4 vn_;
};

class ObjParser
{
private:
	std::vector<std::array<float, 4>>	geometricVertices_;
	std::vector<std::array<float, 3>>	vertexNormals_;
	std::vector<std::array<float, 3>>	textureCoordinates_;
	std::vector<std::array<float, 3>>	parameterSpaceVertices_;

	std::vector<FaceElement>			faceElements_;
	void								generateFaceElements(const std::vector<std::string>& faceList);

public:
	ObjParser(const std::string& src);
	ObjParser() = default;
	~ObjParser() = default;

	//List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.0.
	std::vector<std::array<float, 4>> getGeometricVerticies()		const { return geometricVertices_; }
	// List of vertex normals in (x,y,z) form; normals might not be unit vectors.
	std::vector<std::array<float, 3>> getVertexNormals()			const { return vertexNormals_; }
	// List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.
	std::vector<std::array<float, 3>> getTextureCoordinates()		const { return textureCoordinates_; }
	// Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement
	std::vector<std::array<float, 3>> getParameterSpaceVertices()	const { return parameterSpaceVertices_; }

	// get faceVertex described by string, "vIndex/vtIndex/vnIndex/" example: "1/1/1"
	FaceElement getFaceVertex(const std::string& desc) const;
	std::vector<FaceElement> getFaceElements() const { return faceElements_; };
};

