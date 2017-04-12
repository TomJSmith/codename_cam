#include "navMesh.h"

#include <IOStream>
#include "NavNode.h"
#include <assimp\scene.h>
#include <assimp\material.h>
#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <algorithm>

#include "Util.h"

namespace py = boost::python;

NavMesh::NavMesh(const char* navMeshFileName, glm::vec3 aScale)
{
	scale = aScale;
	Assimp::Importer importer;
	auto fullfilename = Util::ModelDirectory + navMeshFileName;
	const aiScene* navMeshScene = importer.ReadFile(fullfilename, aiProcess_JoinIdenticalVertices);
	if (!navMeshScene) {
		std::cerr << "could not load nav mesh: " << navMeshFileName << "\n" << importer.GetErrorString() << std::endl;
	}
	mNavMesh = navMeshScene->mMeshes[0];
	/*std::cout << "imported file " << navMeshFileName << std::endl;
	std::cout << "num of faces: " << mNavMesh->mNumFaces << std::endl;
	std::cout << "num of verts: " << mNavMesh->mNumVertices << std::endl;*/
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
			xSum += mNavMesh->mVertices[navFace.mIndices[j]].x * scale.x;
			zSum += mNavMesh->mVertices[navFace.mIndices[j]].z * scale.z;
		}

		nodeGraph.push_back(NavNode((xSum / navFace.mNumIndices), (zSum / navFace.mNumIndices), &navFace, mNavMesh->mVertices, &scale));

	}
	// for every node in the graph
	for (int i = 0; i < nodeGraph.size(); i++)
	{
		NavNode& aNode = nodeGraph[i];
		// for all other nodes not yet looked at
		for (int j = i + 1; j < nodeGraph.size(); j++)
		{
			NavNode& possibleNeighbour = nodeGraph[j];
			// for each vert of the first node
			int sharedPointCount = 0;
			for(int k = 0; k < aNode.vertIndices.size(); k++)
			{
				bool isShared = std::any_of(std::begin(possibleNeighbour.vertIndices), std::end(possibleNeighbour.vertIndices), [&](uint32_t i)
				{
					return i == aNode.vertIndices[k];
				});
				if (isShared)
				{
					sharedPointCount++;
				}
			}
			if (sharedPointCount > 1)
			{
				aNode.addNeighbour(&possibleNeighbour);
				possibleNeighbour.addNeighbour(&aNode);
			}
		}
	}
	for (int i = 0; i < nodeGraph.size(); i++)
	{
		std::cout << nodeGraph[i].neighbours.size() << std::endl;
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
		py::list vertices = py::list();
		for (int k = 0; k < nodeGraph[i].vertIndices.size(); k++) {
			float kx = nodeGraph[i].mVerts[nodeGraph[i].vertIndices[k]].x * nodeGraph[i].scale->x;
			float kz = nodeGraph[i].mVerts[nodeGraph[i].vertIndices[k]].z * nodeGraph[i].scale->z;
			py::tuple vert = py::make_tuple(kx, kz);
			vertices.append(vert);
		}
		py::tuple node = py::make_tuple(x, z, neighbors, vertices);
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
