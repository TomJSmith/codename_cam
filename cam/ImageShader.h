#pragma once

#include "Shader.h"

#include "Texture.h"

class ImageShader : public Shader
{
public:
	ImageShader(const char *filename);
	virtual void Setup(const Renderer::MeshData &data, const mat4 &projection) override;

private:
	Texture texture_;
};

