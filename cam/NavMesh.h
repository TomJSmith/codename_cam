#pragma once

#include "assimp\scene.h"
#include "NavNode.h"
#include <vector>

class NavMesh
{
public:
	NavMesh(const char* navmeshFileName);
	~NavMesh();
	std::vector<NavNode> nodeGraph;

private:
	aiMesh* mNavMesh;
	void process();
};
