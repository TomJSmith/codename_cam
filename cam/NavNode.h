#pragma once

#include <vector>
#include "assimp\scene.h"
#include "glm\glm.hpp"

class NavNode
{
public:
	NavNode(float x, float y, const aiFace* aFace, const aiVector3D* verts, const glm::vec3* aScale);
	~NavNode();
	std::vector<NavNode*> neighbours;
	float centerX;
	float centerZ;
	const aiFace* mFace;
	const aiVector3D* mVerts;
	std::vector<uint32_t> vertIndices;
	const glm::vec3* scale;

	void addNeighbour(NavNode* neighbour);
	bool isInside(float xPos, float zPos);
	float distance(NavNode* aNode);
private:

};
