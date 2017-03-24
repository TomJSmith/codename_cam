#pragma once

#include "Component.h"

#include <functional>

#include "Events.h"
#include "ImageShader.h"

class Image : public Component
{
public:
	Image(const char *filename, const vec2 &pos, const vec2 &size, size_t layer = 1);

	void Destroy() override;
	void SetPosition(const vec2 &pos);
	void SetSize(const vec2 &size);
	void SetLayer(size_t layer);
	vec2 GetPosition() { return pos_; }
	vec2 GetSize() { return size_; }

protected:
	void RegisterHandlers() override;
	void GenerateBuffers();
	void FillBuffers();
private:
	ImageShader shader_;
	std::function<void(Events::Render)> handler_;
	GLuint vao_;
	GLuint buffers_[2];

	vec2 pos_;
	vec2 size_;
};

