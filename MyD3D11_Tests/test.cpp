#include "pch.h"
#include "../MyD3D11Project/Entity.cpp"

class Entity_readFromFile : public testing::Test 
{
protected:
	Entity m_entity;

	void SetUp() override
	{
		m_entity = Entity();
	}
};

TEST_F(Entity_readFromFile, positionEntries) 
{
	EXPECT_EQ(m_entity.getNrOfVertices(), 8);
}

TEST_F(Entity_readFromFile, normalEntries)
{
	EXPECT_EQ(m_entity.getNrOfNormals(), 6);
}

TEST_F(Entity_readFromFile, faceEntries)
{
	EXPECT_EQ(m_entity.getNrOfFaces(), 12);
}

TEST_F(Entity_readFromFile, materialObjects)
{
	EXPECT_EQ(m_entity.getNrOfMtlObj(), 1);
}