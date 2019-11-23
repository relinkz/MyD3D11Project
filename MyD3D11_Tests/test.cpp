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
		{ 0.100000, 0.110000, 0.111000 },
		{ 0.200000, 0.220000, 0.222000 },
		{ 0.300000, 0.330000, 0.333000 },
		{ 0.400000, 0.440000, 0.444000 },
		{ 0.500000, 0.550000, 0.555000 },
		{ 0.600000, 0.660000, 0.666000 },
		{ 0.700000, 0.770000, 0.777000 },
		{ 0.800000, 0.880000, 0.888000 },
	};

	ASSERT_EQ(expected, test.getParameterSpaceVertices());
}