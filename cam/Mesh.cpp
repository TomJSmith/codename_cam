#include "System.h"

#include "Mesh.h"

#include "Entity.h"
#include "Shader.h"
#include <IOStream>

#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\material.h>
#include <assimp\Importer.hpp>

Mesh::Mesh(std::shared_ptr<Shader> shader,
		   const char* objFileName,	
		   glm::vec3 colour,
		   glm::vec3 scale,
		   GLuint type) :
	shader_(std::move(shader)),
	type_(type)
{
	Assimp::Importer importer;
	auto fullobjfilename = Util::ModelDirectory + objFileName;
	const aiScene* objFile = importer.ReadFile(fullobjfilename, aiProcessPreset_TargetRealtime_MaxQuality);
	if (!objFile)
	{
		std::cerr << "could not load file " << fullobjfilename << ": " << importer.GetErrorString() << std::endl;
	}

	/*std::cout << fullobjfilename << std::endl;
	std::cout << "num of textures: " << objFile->mNumMaterials << std::endl;
	std::cout << "num of meshes: " << objFile->mNumMeshes << std::endl;*/
	std::vector<aiVector3D> vertices;
	std::vector<aiVector3D> normals;
	std::vector<aiVector2D> texCoords;
	std::vector<glm::vec3> colours;
	aiMesh* objMesh = objFile->mMeshes[0];
	aiMaterial* objMaterial = objFile->mMaterials[objMesh->mMaterialIndex];
	/*std::cout << "num of faces: " << objMesh->mNumFaces << std::endl;
	std::cout << "num of verts: " << objMesh->mNumVertices << std::endl;
	std::cout << "num materials: " << objFile->mNumMaterials << std::endl;*/
	for (std::uint32_t i = 0u; i < objMesh->mNumFaces; i++)
	{
		const aiFace& face = objMesh->mFaces[i];
		for (uint32_t j = 0; j < 3; j++)
		{

			aiVector3D vert = objMesh->mVertices[face.mIndices[j]];
			aiVector3D faceVert(vert.x * scale.x, vert.y * scale.y, vert.z * scale.z);
			vertices.push_back(faceVert);

			aiVector3D norm = objMesh->mNormals[face.mIndices[j]];
			aiVector3D faceNormal(norm.x * scale.x, norm.y * scale.y, norm.z * scale.z);
			normals.push_back(faceNormal);

			aiVector3D texCoord = objMesh->mTextureCoords[0][face.mIndices[j]];
			texCoords.push_back(aiVector2D(texCoord.x, texCoord.y));
			colours.push_back(colour);
		}
	}

	const aiMaterial* m_material = objFile->mMaterials[0];
	if (m_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString relTexPath;
		if (m_material->GetTexture(aiTextureType_DIFFUSE, 0, &relTexPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			
			//std::cout << "success!" << std::endl;
		}
	}

	count_ = objMesh->mNumFaces * 3;

	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(aiVector3D), vertices.data(), GL_STATIC_DRAW);

	GLuint colourBuffer = 0;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(glm::vec3), colours.data(), GL_STATIC_DRAW);

	GLuint normalBuffer = 0;
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(aiVector3D), normals.data(), GL_STATIC_DRAW);

	GLuint texCoordBuffer = 0;
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, count_ * sizeof(aiVector2D), texCoords.data(), GL_STATIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::GetMeshData(Events::Render event)
{
	event.data.push_back({
		vao_,
		count_,
		type_,
		entity_->GetGlobalTransform(),
		shader_.get()
	});
}

void Mesh::RegisterHandlers()
{
	handler_ = [this] (Events::Render e) {
		GetMeshData(e);
	};

	entity_->RegisterEventHandler(&handler_);
}

void Mesh::Destroy()
{
	entity_->UnregisterEventHandler(&handler_);
}