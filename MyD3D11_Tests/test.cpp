#include "pch.h"
#include "../MyD3D11Project/ObjParser.h"
#include "../MyD3D11Project/ObjParser.cpp"
//TODO: linker are searching for the cpp file inside testproject.

using namespace std;

TEST(ObjParser, GeoVerts)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	vector<array<float, 4>> expectedGeoVerts
	{
		{ -0.500000, -0.500000,	0.500000,  1.000000 },
		{  0.500000, -0.500000, 0.500000,  1.000000 },
		{ -0.500000, 0.500000,  0.500000,  1.000000 },
		{  0.500000, 0.500000,  0.500000,  1.000000 },

		{ -0.500000,  0.500000, -0.500000 , 1.000000},
		{  0.500000,  0.500000, -0.500000 , 1.000000},
		{ -0.500000, -0.500000, -0.500000 , 1.000000},
		{  0.500000, -0.500000, -0.500000 , 1.000000}
	};

	ASSERT_EQ(expectedGeoVerts, test.getGeometricVerticies());
}

TEST(ObjParser, vertexNormals)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	vector<array<float, 3>> expectedVertexNormals
	{
		{ 0.000000, 0.000000, 1.000000 },
		{ 0.000000, 1.000000, 0.000000 },
		{ 0.000000, 0.000000, -1.000000 },

		{ 0.000000, -1.000000, 0.000000 },
		{ 1.000000, 0.000000, 0.000000 },
		{ -1.000000, 0.000000, 0.000000 }
	};

	ASSERT_EQ(expectedVertexNormals, test.getVertexNormals());
}

TEST(ObjParser, textureCoordinates)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	vector<array<float, 3>> expected
	{
		{ 0.000000, 0.000000, 0.000000 },
		{ 1.000000, 0.000000, 0.000000 },
		{ 0.000000, 1.000000, 0.000000 },
		{ 1.000000, 1.000000, 0.000000 }
	};

	ASSERT_EQ(expected, test.getTextureCoordinates());
}

TEST(ObjParser, parameterSpaceVertices)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	vector<array<float, 3>> expected
	{
		{ 0.100000f, 0.110000f, 0.111000f },
		{ 0.200000f, 0.220000f, 0.222000f },
		{ 0.300000f, 0.330000f, 0.333000f },
		{ 0.400000f, 0.440000f, 0.444000f },
		{ 0.500000f, 0.550000f, 0.555000f },
		{ 0.600000f, 0.660000f, 0.666000f },
		{ 0.700000f, 0.770000f, 0.777000f },
		{ 0.800000f, 0.880000f, 0.888000f },
	};

	ASSERT_EQ(expected, test.getParameterSpaceVertices());
}

TEST(ObjParser, generateFacesSampleFirst)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	// f 1/1/1 2/1/1 3/1/1
	FaceElement expected;
	expected.v_ = { -0.500000, -0.500000,	0.500000,  1.000000 };
	expected.vt_ = { 0.000000, 0.000000, 0.000000, 0.0 };
	expected.vn_ = { 0.000000, 0.000000, 1.000000, 0.0 };

	std::vector<FaceElement> faceElem = test.getFaceElements();

	ASSERT_EQ(expected.v_.x, faceElem.at(0).v_.x);
	ASSERT_EQ(expected.v_.y, faceElem.at(0).v_.y);
	ASSERT_EQ(expected.v_.z, faceElem.at(0).v_.z);
	ASSERT_EQ(expected.v_.w, faceElem.at(0).v_.w);

	ASSERT_EQ(expected.vt_.x, faceElem.at(0).vt_.x);
	ASSERT_EQ(expected.vt_.y, faceElem.at(0).vt_.y);
	ASSERT_EQ(expected.vt_.z, faceElem.at(0).vt_.z);
	ASSERT_EQ(expected.vt_.w, faceElem.at(0).vt_.w);

	ASSERT_EQ(expected.vn_.x, faceElem.at(0).vn_.x);
	ASSERT_EQ(expected.vn_.y, faceElem.at(0).vn_.y);
	ASSERT_EQ(expected.vn_.z, faceElem.at(0).vn_.z);
	ASSERT_EQ(expected.vn_.w, faceElem.at(0).vn_.w);
}

TEST(ObjParser, generateFacesSampleLast)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	// f 3/1/6
	FaceElement expected;
	expected.v_ = { -0.500000, 0.500000, 0.500000,  1.000000 };
	expected.vt_ = { 0.000000, 0.000000, 0.000000, 0.0 };
	expected.vn_ = { -1.000000, 0.000000, 0.000000, 0.0 };

	std::vector<FaceElement> faceElem = test.getFaceElements();

	ASSERT_EQ(expected.v_.x, faceElem.back().v_.x);
	ASSERT_EQ(expected.v_.y, faceElem.back().v_.y);
	ASSERT_EQ(expected.v_.z, faceElem.back().v_.z);
	ASSERT_EQ(expected.v_.w, faceElem.back().v_.w);

	ASSERT_EQ(expected.vt_.x, faceElem.back().vt_.x);
	ASSERT_EQ(expected.vt_.y, faceElem.back().vt_.y);
	ASSERT_EQ(expected.vt_.z, faceElem.back().vt_.z);
	ASSERT_EQ(expected.vt_.w, faceElem.back().vt_.w);

	ASSERT_EQ(expected.vn_.x, faceElem.back().vn_.x);
	ASSERT_EQ(expected.vn_.y, faceElem.back().vn_.y);
	ASSERT_EQ(expected.vn_.z, faceElem.back().vn_.z);
	ASSERT_EQ(expected.vn_.w, faceElem.back().vn_.w);
}

TEST(ObjParser, ObjMtlData)
{
	ObjParser test("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/crate1.obj");

	stringContainer expectedMtlFiles			= { "crate1.mtl" };
	stringContainer expectedMtlGroupNames		= { "default", "crate1" };
	stringContainer expectedMtlNames			= { "M_crate1_blinn" };
	stringContainer expectedMtlSmoothShadings	= { "off" };

	ASSERT_EQ(expectedMtlFiles,				test.getMtlFiles());
	ASSERT_EQ(expectedMtlGroupNames,		test.getMtlGroupNames());
	ASSERT_EQ(expectedMtlNames,				test.getMtlNames());
	ASSERT_EQ(expectedMtlSmoothShadings,	test.getMtlSmoothShading());
}

TEST(ObjParser, mtlSplitter)
{
	stringContainer mtlFile
	{
		"newmtl M_crate1_blinn",
		"illum 4",
		"Kd 0.00 0.00 0.00",
		"Ka 0.00 0.00 0.00",
		"Tf 1.00 1.00 1.00",
		"map_Kd T_crate1_D.tga",
		"bump T_crate1_N.tga - bm T_crate1_N.tga",
		"Ni 1.00",
		"Ks 0.50 0.50 0.50",
		"map_Ks T_crate1_S.tga"
	};

	std::vector<stringContainer> expected;
	expected.push_back(mtlFile);

	ASSERT_EQ(expected, splitByMaterials(mtlFile));
}

TEST(ObjParser, mtlSplitter2)
{
	stringContainer mtlFile
	{
		"newmtl M_crate1_blinn",
		"illum 4",
		"Kd 0.00 0.00 0.00",
		"Ka 0.00 0.00 0.00",
		"Tf 1.00 1.00 1.00",
		"map_Kd T_crate1_D.tga",
		"bump T_crate1_N.tga - bm T_crate1_N.tga",
		"Ni 1.00",
		"Ks 0.50 0.50 0.50",
		"map_Ks T_crate1_S.tga",
		"newmtl M_crate2_blinn",
		"illum 4",
		"Kd 0.00 0.00 0.00",
		"Ka 0.00 0.00 0.00",
		"Tf 1.00 1.00 1.00",
		"map_Kd T_crate1_D.tga",
		"bump T_crate1_N.tga - bm T_crate1_N.tga",
		"Ni 1.00",
		"Ks 0.50 0.50 0.50",
		"map_Ks T_crate1_S.tga"
	};

	stringContainer exp1
	{
		"newmtl M_crate1_blinn",
		"illum 4",
		"Kd 0.00 0.00 0.00",
		"Ka 0.00 0.00 0.00",
		"Tf 1.00 1.00 1.00",
		"map_Kd T_crate1_D.tga",
		"bump T_crate1_N.tga - bm T_crate1_N.tga",
		"Ni 1.00",
		"Ks 0.50 0.50 0.50",
		"map_Ks T_crate1_S.tga"
	};	
	stringContainer exp2
	{
		"newmtl M_crate2_blinn",
		"illum 4",
		"Kd 0.00 0.00 0.00",
		"Ka 0.00 0.00 0.00",
		"Tf 1.00 1.00 1.00",
		"map_Kd T_crate1_D.tga",
		"bump T_crate1_N.tga - bm T_crate1_N.tga",
		"Ni 1.00",
		"Ks 0.50 0.50 0.50",
		"map_Ks T_crate1_S.tga"
	};

	std::vector<stringContainer> expected;
	expected.push_back(exp1);
	expected.push_back(exp2);

	ASSERT_EQ(expected, splitByMaterials(mtlFile));
}