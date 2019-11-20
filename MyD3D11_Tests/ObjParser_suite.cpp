#include "pch.h"
#include "../MyD3D11Project/ObjParser.cpp"

using container = std::vector<std::string>;

TEST(ObjParser_storeFileAsString, test1)
{
	container e;

	e.emplace_back("# a comment");
	e.emplace_back("");
	e.emplace_back("v -0.500000 -0.500000 0.500000");
	e.emplace_back("");
	e.emplace_back("# a comment");
	e.emplace_back("vt 0.000000 0.000000");
	e.emplace_back("");
	e.emplace_back("vn 0.000000 0.000000 1.000000");

	ASSERT_EQ(e, storeFileAsString("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/test1.obj"));
}

class ObjParser_fetchAllStringsOfType : public testing::Test
{
protected:
	container testData_;

	void SetUp() override
	{
		testData_.emplace_back("# a comment");
		testData_.emplace_back("");
		testData_.emplace_back("v -0.500000 -0.500000 0.500000");
		testData_.emplace_back("");
		testData_.emplace_back("# a comment");
		testData_.emplace_back("vt 0.000000 0.000000");
		testData_.emplace_back("");
		testData_.emplace_back("v -0.500000 -0.500000 0.500000");
		testData_.emplace_back("v -0.54242 -5367.00000 0.999999");
		testData_.emplace_back("vn 0.000000 0.000000 1.000000");
		testData_.emplace_back("vp 0.310000 3.210000 2.100000");
	}
};

TEST_F(ObjParser_fetchAllStringsOfType, geometricVertices)
{
	container geoVerts = fetchAllStringsOfType("v ", testData_);
	
	container expect;
	expect.emplace_back("-0.500000 -0.500000 0.500000");
	expect.emplace_back("-0.500000 -0.500000 0.500000");
	expect.emplace_back("-0.54242 -5367.00000 0.999999");

	ASSERT_EQ(expect, geoVerts);
}

TEST_F(ObjParser_fetchAllStringsOfType, neg_geometricVertices)
{
	container geoVerts = fetchAllStringsOfType("v ", testData_);
	container expect;

	expect.emplace_back("-0.500000 -0.500000 0.500000");
	expect.emplace_back("-0.500000 -0.500000 0.500000");
		
	ASSERT_NE(expect, geoVerts);
}

TEST_F(ObjParser_fetchAllStringsOfType, vertexNormals)
{
	container normVerts = fetchAllStringsOfType("vn ", testData_);
	container expect;

	expect.emplace_back("0.000000 0.000000 1.000000");

	ASSERT_EQ(expect, normVerts);
}

TEST_F(ObjParser_fetchAllStringsOfType, neg_vertexNormals)
{
	container normVerts = fetchAllStringsOfType("vn ", testData_);
	container expect;

	expect.emplace_back("0.000000 1.000000 0.000000");

	ASSERT_NE(expect, normVerts);
}

TEST_F(ObjParser_fetchAllStringsOfType, parameterSpaceVerts)
{
	container paraSpaceVerts = fetchAllStringsOfType("vp ", testData_);
	container expect;

	expect.emplace_back("0.310000 3.210000 2.100000");

	ASSERT_EQ(expect, paraSpaceVerts);
}

TEST_F(ObjParser_fetchAllStringsOfType, neg_parameterSpaceVerts)
{
	container paraSpaceVerts = fetchAllStringsOfType("vp ", testData_);
	container expect;
	
	expect.emplace_back("3.310000 3.210000 2.100000");

	ASSERT_NE(expect, paraSpaceVerts);
}

TEST_F(ObjParser_fetchAllStringsOfType, textureCoords)
{
	container textCoords = fetchAllStringsOfType("vt ", testData_);
	container expect;

	expect.emplace_back("0.000000 0.000000");

	ASSERT_EQ(expect, textCoords);
}

TEST_F(ObjParser_fetchAllStringsOfType, neg_textureCoords)
{
	container textCoords = fetchAllStringsOfType("vt ", testData_);
	container expect;

	expect.emplace_back("0.000000 1.100000");

	ASSERT_NE(expect, textCoords);
}

TEST_F(ObjParser_fetchAllStringsOfType, comments)
{
	container comm = fetchAllStringsOfType("# ", testData_);
	container expect;

	expect.emplace_back("a comment");
	expect.emplace_back("a comment");

	ASSERT_EQ(expect, comm);
}

TEST_F(ObjParser_fetchAllStringsOfType, neg_comments)
{
	container comm = fetchAllStringsOfType("# ", testData_);
	container expect;

	expect.emplace_back("a comment");
	expect.emplace_back("a faulty comment");

	ASSERT_NE(expect, comm);
}

class ObjParser_splitStringByCharacter : public testing::Test
{
protected:
	// ordinary vertexes, x y z (a)
	string vertexType1_ = "-0.100000 -0.200000 -0.300000";
	stringContainer expectedVertexType1_ = { "-0.100000", "-0.200000", "-0.300000" };

	string vertexType2_ = "0.1 0.2 0.3";
	stringContainer expectedVertexType2_ = { "0.1", "0.2","0.3" };

	stringContainer expectedVertexType3_ = { "0.1", "0.2", "0.3", "0.4" };
	string vertexType3_ = "0.1 0.2 0.3 0.4";

	// face element, [geoVector index / texture index / normal index]
	string polynomialType1_ = "1 2 3";
	stringContainer expectedPolynomialType1_ = { "1", "2", "3" };
	string polynomialType2_ = "3/1 4/2 5/3";
	stringContainer expectedPolynomialType2_ = { "3/1", "4/2", "5/3" };

	string polynomialType3_ = "6/4/1 3/5/3 7/6/5";
	stringContainer expectedPolynomialType3_ = { "6/4/1", "3/5/3", "7/6/5" };

	string polynomialType4_ = "7//1 8//2 9//3";
	stringContainer expectedPolynomialType4_ = { "7//1", "8//2", "9//3" };

	// line, vector indexes describing the line
	string lineElement_ = "5 8 1 2 4 9";
	stringContainer expectedLineElement_ = { "5", "8", "1", "2", "4", "9" };
};

TEST_F(ObjParser_splitStringByCharacter, whitespace)
{
	ASSERT_EQ(splitStringByCharacter(vertexType1_, ' '), expectedVertexType1_);
	ASSERT_EQ(splitStringByCharacter(vertexType2_, ' '), expectedVertexType2_);
	ASSERT_EQ(splitStringByCharacter(vertexType3_, ' '), expectedVertexType3_);

	ASSERT_EQ(splitStringByCharacter(polynomialType1_, ' '), expectedPolynomialType1_);
	ASSERT_EQ(splitStringByCharacter(polynomialType2_, ' '), expectedPolynomialType2_);
	ASSERT_EQ(splitStringByCharacter(polynomialType3_, ' '), expectedPolynomialType3_);
	ASSERT_EQ(splitStringByCharacter(polynomialType4_, ' '), expectedPolynomialType4_);

	ASSERT_EQ(splitStringByCharacter(lineElement_, ' '), expectedLineElement_);
}

TEST_F(ObjParser_splitStringByCharacter, frontslash)
{
	vector<stringContainer> elements;
	for (const string& element : expectedPolynomialType2_)
		elements.emplace_back(splitStringByCharacter(element, '/'));

	stringContainer vt0 = { "3", "1" };
	stringContainer vt1 = { "4", "2" };
	stringContainer vt2 = { "5", "3" };

	ASSERT_EQ(elements[0], vt0);
	ASSERT_EQ(elements[1], vt1);
	ASSERT_EQ(elements[2], vt2);
}

TEST(Constructor, file1)
{
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

	ObjParser obj = ObjParser("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/testObj.obj");

	ASSERT_EQ(obj.getGeometricVerticies(), expectedGeoVerts);
}