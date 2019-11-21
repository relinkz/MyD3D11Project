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
		{ -0.500000, -0.500000,	0.500000, 0.000000 },
		{ 0.500000,	-0.500000, 0.500000, 0.000000 },
		{ -0.500000, 0.500000, 0.500000, 0.000000 },
		{ 0.500000, 0.500000, 0.500000, 0.000000 },

		{ -0.500000, 0.500000, -0.500000 , 0.000000},
		{ 0.500000, 0.500000, -0.500000 , 0.000000},
		{ -0.500000, -0.500000, -0.500000 , 0.000000},
		{ 0.500000, -0.500000, -0.500000 , 0.000000}
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

	vector<array<float, 2>> expected
	{
		{ 0.000000, 0.000000 },
		{ 1.000000, 0.000000 },
		{ 0.000000, 1.000000 },
		{ 1.000000, 1.000000 }
	};

	ASSERT_EQ(expected, test.getTextureCoordinates());
}