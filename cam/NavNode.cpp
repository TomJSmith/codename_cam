#include "NavNode.h"
#include <math.h>
#include <iostream>

NavNode::NavNode(float x, float z, const aiFace* aFace)
{
	centerX = x;
	centerZ = z;
	mFace = aFace;
	for (uint32_t i = 0; i < mFace->mNumIndices; i++)
	{
		vertIndices.push_back(mFace->mIndices[i]);
	}
}

NavNode::~NavNode()
{
}

void NavNode::addNeighbour(NavNode* neighbour)
{
	//std::cout << "node at " << neighbour->centerX << ",  " << neighbour->centerZ << " added to node at " << centerX << ", " << centerZ << std::endl;
	neighbours.push_back(neighbour);
}

float NavNode::distance(NavNode* aNode)
{
	float dx = centerX - aNode->centerX;
	float dz = centerX - aNode->centerZ;
	return sqrt((dx * dx) + (dz * dz));
}