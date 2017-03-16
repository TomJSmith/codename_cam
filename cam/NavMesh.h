#pragma once

#include "assimp\scene.h"
#include "NavNode.h"
#include <vector>
#include <boost/python.hpp>

namespace py = boost::python;

class NavMesh
{
public:
	NavMesh(const char* navmeshFileName);
	~NavMesh();
	std::vector<NavNode> nodeGraph;
	py::list getSimpleGraph();
	//py::list getSimpleNeighbors(py::tuple node);
private:
	aiMesh* mNavMesh;
	void process();
};
