#include "System.h"

#include "Mesh.h"

#include "Entity.h"
#include "Shader.h"

Mesh::Mesh(Shader &shader,
		   GLuint nvertices,
		   std::vector<glm::vec3> vertices,
		   std::vector<glm::vec3> normals,
		   std::vector<glm::vec3> colours,
		   GLuint type) :
	shader_(shader),
	type_(type),
	count_(nvertices)
{
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	GLuint colourBuffer = 0;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(glm::vec3), colours.data(), GL_STATIC_DRAW);

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