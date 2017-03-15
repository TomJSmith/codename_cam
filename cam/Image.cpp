#include "Image.h"

#include "Entity.h"

Image::Image(const char *filename) :
	shader_(filename)
{
	std::vector<vec2> positions = {
		{ 0.5f, 0.5f },
		{-0.5f, 0.5f},
		{ -0.5f, -0.5f },
		{ -0.5f, -0.5f },
		{0.5f, -0.5f},
		{0.5f, 0.5f},
	};

	std::vector<vec2> uvs = {
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
	};

	GLuint buffers[2] = {0};

	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	handler_ = [this](Events::Render event) {
		event.data.push_back({
			vao_,
			6,
			GL_TRIANGLES,
			mat3(1.0), // TODO
			&shader_
		});
	};
}

void Image::RegisterHandlers()
{
	entity_->RegisterEventHandler(&handler_);
}

void Image::Destroy()
{
	entity_->UnregisterEventHandler(&handler_);
}