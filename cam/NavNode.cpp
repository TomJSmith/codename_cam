#include "NavNode.h"
#include <math.h>
#include <iostream>

NavNode::NavNode(float x, float z, const aiFace* aFace, const aiVector3D* verts)
{
	centerX = x;
	centerZ = z;
	mFace = aFace;
	mVerts = verts;
	for (uint32_t i = 0; i < mFace->mNumIndices; i++)
	{
		std::cout << mFace->mIndices[i] << std::endl;
		/*vertIndices.push_back(aiVector2D(aMesh->mVertices[aFace->mIndices[i]].x,
			aMesh->mVertices[aFace->mIndices[i]].z));*/
		vertIndices.push_back(mFace->mIndices[i]);
	}
}

NavNode::~NavNode()
{
}

void NavNode::addNeighbour(NavNode* neighbour)
{
	neighbours.push_back(neighbour);
}

float NavNode::distance(NavNode* aNode)
{
	float dx = centerX - aNode->centerX;
	float dz = centerX - aNode->centerZ;
	return sqrt((dx * dx) + (dz * dz));
}

bool NavNode::isInside(float xPos, float zPos)
{
	int i;
	int j;
	bool result = false;
	for (i = 0, j = (int)vertIndices.size() - 1; i < vertIndices.size(); j = i++)
	{
		float ix = mVerts[vertIndices[i]].x;
		float iz = mVerts[vertIndices[i]].z;
		float jx = mVerts[vertIndices[j]].x;
		float jz = mVerts[vertIndices[j]].z;
		if ((iz > zPos) != (jz > zPos) &&
			(xPos < (jx - ix) * (zPos - iz) / (jz - iz) + ix))
		{
			result = !result;
		}
	}
	std::cout << xPos << ",   " << zPos << "    " << result << std::endl;
	return result;
}