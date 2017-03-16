#pragma once

#include "Shader.h"

class ModelShader :	public Shader
{
public:
	ModelShader();
	virtual void Setup(const Renderer::MeshData &data, const mat4 &projection) override;
};