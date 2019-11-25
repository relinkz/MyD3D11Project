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

enum IlluminationModel
{
	colorOnAmbientOff,
	colorOnAmbientOn,
	highlightOn,
	unknown
};

struct Material
{
	Material() = default;
	std::string			materialName_		= "";
	DirectX::XMFLOAT3	ambientColor_		= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3	diffuseColor_		= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3	specularColor_		= { 0.0f, 0.0f, 0.0f };
	uint16_t			specularExponent_	= 0;
	IlluminationModel	illuminationModel_	= IlluminationModel::unknown;

	// todo: use std::optional
	std::string ambient = "";
	std::string diffuse = "";
	std::string specular = "";
	std::string specularHighlight = "";
	std::string alpha = "";
	std::string bumpMap = "";
	std::string displacement = "";
	std::string stencilDecal = "";
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

	// material definitions
	std::vector<std::string>			mtlFiles_;
	std::vector<std::string>			mtlNames_;

	std::vector<Material>				materials_;
	void ObjParser::readMtlLibrary(const string& libPath);

	// grouping, unused by this application until I find some use for it
	std::vector<std::string>			mtlGroupNames_;
	std::vector<std::string>			mtlSmoothShading_;

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
	
	std::vector<FaceElement> getFaceElements()		const { return faceElements_; }
	std::vector<std::string> getMtlFiles()			const { return mtlFiles_; }
	std::vector<std::string> getMtlNames()			const { return mtlNames_; }
	std::vector<std::string> getMtlGroupNames()		const { return mtlGroupNames_; }
	std::vector<std::string> getMtlSmoothShading()	const { return mtlSmoothShading_; }

};

