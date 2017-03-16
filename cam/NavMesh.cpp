#include "navMesh.h"

#include <IOStream>
#include "NavNode.h"
#include <assimp\scene.h>
#include <assimp\material.h>
#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <algorithm>

namespace py = boost::python;


NavMesh::NavMesh(const char* navMeshFileName)
{
	Assimp::Importer importer;
	const aiScene* navMeshScene = importer.ReadFile(navMeshFileName, aiProcess_JoinIdenticalVertices);
	if (!navMeshScene) {
		std::cerr << "could not load nav mesh: " << navMeshFileName << "\n" << importer.GetErrorString() << std::endl;
	}
	mNavMesh = navMeshScene->mMeshes[0];
	std::cout << "imported file " << navMeshFileName << std::endl;
	std::cout << "num of faces: " << mNavMesh->mNumFaces << std::endl;
	std::cout << "num of verts: " << mNavMesh->mNumVertices << std::endl;
	process();
}

NavMesh::~NavMesh()
{

}

void NavMesh::process()
{
	for (uint32_t i = 0; i < mNavMesh->mNumFaces; i++)
	{
		const aiFace& navFace = mNavMesh->mFaces[i];
		float xSum = 0;
		float zSum = 0;
		for (uint32_t j = 0; j < navFace.mNumIndices; j++)
		{
			xSum += mNavMesh->mVertices[navFace.mIndices[j]].x;
			zSum += mNavMesh->mVertices[navFace.mIndices[j]].z;
		}
		nodeGraph.push_back(NavNode((xSum / navFace.mNumIndices), (zSum / navFace.mNumIndices), &navFace));
	}
	for (int i = 0; i < nodeGraph.size(); i++)
	{
		NavNode& aNode = nodeGraph[i];
		for (int j = i + 1; j < nodeGraph.size(); j++)
		{
			NavNode& possibleNeighbour = nodeGraph[j];
			for (int k = 0; k < aNode.vertIndices.size(); k++)
			{
				bool isNeighbour = std::any_of(std::begin(possibleNeighbour.vertIndices), std::end(possibleNeighbour.vertIndices), [&](uint32_t i)
				{
					return i == aNode.vertIndices[k];
				});
				if (isNeighbour)
				{
					aNode.addNeighbour(&possibleNeighbour);
					possibleNeighbour.addNeighbour(&aNode);
					break;
				}
			}
		}
	}
}
py::list NavMesh::getSimpleGraph() {
	float x, z, nX, nZ;
	int numNeighbors;
	int numNodes = static_cast<int>(nodeGraph.size());
	py::list simple = py::list();
	for (int i = 0; i < numNodes; i++) {
		x = nodeGraph[i].centerX;
		z = nodeGraph[i].centerZ;
		numNeighbors = static_cast<int>(nodeGraph[i].neighbours.size());
		py::list neighbors = py::list();
		for (int j = 0; j < numNeighbors; j++) {
			nX = nodeGraph[i].neighbours[j]->centerX;
			nZ = nodeGraph[i].neighbours[j]->centerZ;
			py::tuple neighbor = py::make_tuple(nX, nZ);
			neighbors.append(neighbor);
		}
		py::tuple node = py::make_tuple(x, z, neighbors);
		simple.append(node);
	}

	return simple;
}

/*py::list NavMesh::getSimpleNeighbors(py::tuple node) {
	float x, z;
	int numNeighbors = static_cast<int>(nodeGraph[nodeNum].neighbours.size());
	py::list neighbors = py::list();
	for (int i = 0; i < numNeighbors; i++) {
		x = nodeGraph[nodeNum].neighbours[i]->centerX;
		z = nodeGraph[nodeNum].neighbours[i]->centerZ;
		py::tuple node = py::make_tuple(x, z);
		neighbors.append(node);
	}
	return neighbors;

}*/
