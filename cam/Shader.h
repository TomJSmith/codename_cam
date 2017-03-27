#pragma once

#include "System.h"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>

#include "Renderer.h"
#include "Util.h"

class Shader
{
public:
	virtual void Setup(const Renderer::MeshData &data, const mat4 &projection) = 0;
	virtual void SetLayer(size_t layer) {}
	virtual ~Shader() = default;

protected:
	Shader(const std::map<GLenum, std::string> &shaders);

	// shader_type is a shared_ptr that uses either glDeleteProgram or glDeleteShader
	// as its deleter, depending on whether it points to a program or a shader
	using shader_type = std::shared_ptr<GLuint>;

	shader_type Compile(GLenum type, const std::string &source);
	shader_type Link(std::set<shader_type> shaders);

	std::set<shader_type> shaders_;
	shader_type program_;

private:
	static std::map<std::string, std::weak_ptr<GLuint>> shaderlist_;
	static std::map<std::set<shader_type>, std::weak_ptr<GLuint>> programlist_;
};

