#pragma once

#include "assimp\scene.h"
#include "NavNode.h"
#include <vector>
#include <boost/python.hpp>
#include "glm\glm.hpp"

namespace py = boost::python;

class NavMesh
{
public:
	NavMesh(const char* navmeshFileName, glm::vec3 aScale);
	~NavMesh();
	std::vector<NavNode> nodeGraph;
	py::list getSimpleGraph();
	//py::list getSimpleNeighbors(py::tuple node);
private:
	aiMesh* mNavMesh;
	void process();
	glm::vec3 scale;
};
