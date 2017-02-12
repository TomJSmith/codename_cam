#include "System.h"

#include "Mesh.h"

#include "Entity.h"
#include "Shader.h"
#include <IOStream>

#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\material.h>
#include <assimp\cimport.h>

Mesh::Mesh(Shader &shader,
		   const char* objFileName,	
		   glm::vec3 colour,
		   GLuint type) :
	shader_(shader),
	type_(type)
{
	const aiScene* objFile = aiImportFile(objFileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (!objFile)
	{
		std::cerr << "could not load file " << objFile << ": " << aiGetErrorString() << std::endl;
	}
	aiMesh* objMesh = objFile->mMeshes[0];
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colours;
	for (std::uint32_t i = 0u; i < objMesh->mNumVertices; i++)
	{
		aiVector3D vert = objMesh->mVertices[i];
		aiVector3D norm = objMesh->mNormals[i];
		vertices.push_back(vec3(vert.x, vert.y, vert.z));
		normals.push_back(vec3(norm.x, norm.y, norm.z));
		colours.push_back(colour);
	}
	count_ = objMesh->mNumVertices;

	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	GLuint colourBuffer = 0;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(glm::vec3), colours.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

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