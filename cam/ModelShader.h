#pragma once

#include "Shader.h"
#include "Texture.h"

class ModelShader :	public Shader
{
public:
	ModelShader();
	ModelShader(const char* filename);
	virtual void Setup(const Renderer::MeshData &data, const mat4 &projection) override;

private:
	Texture texture_;
};