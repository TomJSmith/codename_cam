#include "navMesh.h"

#include <IOStream>
#include "NavNode.h"
#include <assimp\scene.h>
#include <assimp\material.h>
#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <algorithm>


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
			zSum += mNavMesh->mVertices[navFace.mIndices[j]].y;
		}
		nodeGraph.push_back(NavNode(xSum, zSum, &navFace));
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
