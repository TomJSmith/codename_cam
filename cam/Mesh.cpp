#include "System.h"

#include "Mesh.h"

#include "Entity.h"
#include "Shader.h"
#include <IOStream>

#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\material.h>
#include <assimp\Importer.hpp>

Mesh::Mesh(Shader &shader,
		   const char* objFileName,	
		   glm::vec3 colour,
		   float scale,
		   GLuint type) :
	shader_(shader),
	type_(type)
{
	Assimp::Importer importer;
	const aiScene* objFile = importer.ReadFile(objFileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (!objFile)
	{
		std::cerr << "could not load file " << objFile << ": " << importer.GetErrorString() << std::endl;
	}
	std::cout << objFileName << std::endl;
	std::cout << "num of meshes: " << objFile->mNumMeshes << std::endl;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colours;
	aiMesh* objMesh = objFile->mMeshes[0];
	std::cout << "num of faces: " << objMesh->mNumFaces << std::endl;
	std::cout << "num of verts: " << objMesh->mNumVertices << std::endl;
	for (std::uint32_t i = 0u; i < objMesh->mNumFaces; i++)
	{
		const aiFace& face = objMesh->mFaces[i];
		for (int j = 0; j < 3; j++)
		{
			aiVector3D norm = objMesh->mNormals[face.mIndices[j]];
			aiVector3D vert = objMesh->mVertices[face.mIndices[j]];
			if (objFileName == "test_map_mesh.fbx")
			{
				std::cout << vert.x << " " << vert.y << " " << vert.z << std::endl;
			}
			vec3 vertice(vert.x * scale, vert.y * scale, vert.z * scale);
			vec3 normal(norm.x, norm.y, norm.z);
			vertices.push_back(vertice);
			normals.push_back(normal);
			colours.push_back(colour);
		}
	}
	count_ = objMesh->mNumFaces * 3;

	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	GLuint colourBuffer = 0;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(glm::vec3), colours.data(), GL_STATIC_DRAW);

	GLuint normalBuffer = 0;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::GetMeshData(Renderer::RenderEvent event)
{
	event.data.push_back({
		vao_,
		shader_.Program(),
		count_,
		type_,
		entity_->GetGlobalTransform()
	});
}

void Mesh::RegisterHandlers()
{
	handler_ = [this] (Renderer::RenderEvent e) {
		GetMeshData(e);
	};

	// TODO We really need an UnregisterHandlers for when these things
	// are being destroyed or moved between entities
	entity_->GetEvents().RegisterEventHandler(handler_);
}