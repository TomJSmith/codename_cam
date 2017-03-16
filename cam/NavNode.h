#pragma once

#include <vector>
#include "assimp\scene.h"

class NavNode
{
public:
	NavNode(float x, float y, const aiFace* aFace);
	~NavNode();
	std::vector<NavNode*> neighbours;
	float centerX;
	float centerZ;
	const aiFace* mFace;
	std::vector<uint32_t> vertIndices;

	void addNeighbour(NavNode* neighbour);
	float distance(NavNode* aNode);
private:

};
