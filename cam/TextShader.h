#pragma once

#include "Shader.h"

#include "Texture.h"

class TextShader : public Shader
{
public:
	TextShader(unsigned char *data, int width, int height, size_t layer = 1);
	virtual void Setup(const Renderer::MeshData &data, const mat4 &projection) override;
	void SetLayer(size_t layer) { layer_ = layer; }

private:
	Texture texture_;
	size_t layer_;
};

