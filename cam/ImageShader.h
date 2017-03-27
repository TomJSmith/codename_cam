#pragma once

#include "Shader.h"

#include "Texture.h"

class ImageShader : public Shader
{
public:
	ImageShader(const char *filename, size_t layer = 1);
	//ImageShader(unsigned char *data, size_t layer = 1);
	virtual void Setup(const Renderer::MeshData &data, const mat4 &projection) override;
	void SetLayer(size_t layer) { layer_ = layer; }

private:
	Texture texture_;
	size_t layer_;
};

