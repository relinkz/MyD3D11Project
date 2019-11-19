#include "pch.h"
#include "../MyD3D11Project/ObjParser.cpp"

using container = std::vector<std::string>;

TEST(ObjParser_rawReadObj, test1) 
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

	ObjParser parser = ObjParser("C:/Users/seblu/source/repos/MyD3D11Project/MyD3D11_Tests/test1.obj");

	ASSERT_EQ(e, parser.getRawData());
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