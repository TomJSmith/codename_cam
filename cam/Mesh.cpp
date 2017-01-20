#include "OpenGL.h"

#include "Mesh.h"

#include "Entity.h"

Mesh::Mesh(Shader &shader, GLuint nvertices, GLfloat **vertices, GLfloat **colours) :
	shader_(shader)
{
	GLuint vertexBuffer = 0;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(GLfloat) * 3, vertices, GL_STATIC_DRAW);

	GLuint colourBuffer = 0;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(GLfloat) * 3, colours, GL_STATIC_DRAW);

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

void Mesh::GetMeshData(GetMeshDataEvent event)
{
	event.data.push_back({vao_, shader_.Program(), ModelMatrix()});
}

mat4 Mesh::ModelMatrix()
{
	auto model = mat4(1.0f);
	auto e = entity_;

	while (e) {
		model = e->transform().Matrix() * model;
		e = e->Parent();
	}

	return model;
}

void Mesh::RegisterHandlers()
{
	entity_->events().RegisterEventHandler(
		[this](GetMeshDataEvent e) {
			GetMeshData(e);
		}
	);
}