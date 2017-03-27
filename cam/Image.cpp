#include "Image.h"

#include "Entity.h"
#include "ImageShader.h"
#include "TextShader.h"

void Image::GenerateBuffers()
{
	glGenBuffers(2, buffers_);
	glGenVertexArrays(1, &vao_);
}

void Image::FillBuffers()
{
	std::vector<vec2> positions = {
		{ pos_.x, pos_.y },
		{ pos_.x + size_.x, pos_.y + size_.y },
		{ pos_.x, pos_.y + size_.y },
		{ pos_.x, pos_.y },
		{ pos_.x + size_.x, pos_.y },
		{ pos_.x + size_.x, pos_.y + size_.y },
	};

	std::vector<vec2> uvs = {
		{ 0.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
	};


	glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * positions.size(), positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers_[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);

	// TODO do we need to reset all these every time?
	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, buffers_[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers_[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

Image::Image(const char *filename, const vec2 &pos, const vec2 &size, size_t layer) :
	shader_(std::make_shared<ImageShader>(filename, layer)),
	pos_(pos),
	size_(size)
{
	GenerateBuffers();
	FillBuffers();
}

Image::Image(unsigned char *data, int width, int height, const vec2 &pos, const vec2 &size, size_t layer) :
	shader_(std::make_shared<TextShader>(data, width, height, layer)),
	pos_(pos),
	size_(size)
{
	GenerateBuffers();
	FillBuffers();
}

void Image::RegisterHandlers()
{
	handler_ = [this](Events::Render event) {
		event.data.push_back({
			vao_,
			6,
			GL_TRIANGLES,
			mat3(1.0), // TODO
			shader_.get()
		});
	};

	entity_->RegisterEventHandler(&handler_);
}

void Image::Destroy()
{
	entity_->UnregisterEventHandler(&handler_);
}

void Image::SetPosition(const vec2 &pos)
{
	pos_ = pos;
	FillBuffers();
}

void Image::SetSize(const vec2 &size)
{
	size_ = size;
	FillBuffers();
}

void Image::SetLayer(size_t size)
{
	shader_->SetLayer(size);
}