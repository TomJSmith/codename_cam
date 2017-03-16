#include "System.h"

#include <vector>

#include "Shader.h"
#include "Util.h"

std::map<std::string, std::weak_ptr<GLuint>> Shader::shaderlist_;
std::map<std::set<std::shared_ptr<GLuint>>, std::weak_ptr<GLuint>> Shader::programlist_;

std::shared_ptr<GLuint> Shader::Compile(GLenum type, const std::string &source)
{
	auto existing = shaderlist_[source];
	if (!existing.expired()) return existing.lock();

	auto del = [](GLuint *s) { glDeleteShader(*s); delete s; };
	auto shader = std::shared_ptr<GLuint>(new GLuint(glCreateShader(type)), del);

	shaderlist_[source] = shader;

	std::string contents = Util::ReadFile(source);
	const GLchar *src = contents.c_str();
	glShaderSource(*shader, 1, &src, 0);
	glCompileShader(*shader);

	GLint status = GL_FALSE;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length);
		std::string log(length, ' ');
		glGetShaderInfoLog(*shader, length, &length, &log[0]);

		std::string err("failed to compile shader: \n");
		throw std::runtime_error(err + log);
	}

	return shader;
}

std::shared_ptr<GLuint> Shader::Link(std::set<shader_type> shaders)
{
	auto existing = programlist_[shaders];
	if (!existing.expired()) return existing.lock();

	auto del = [](GLuint *p) { glDeleteProgram(*p); delete p; };
	auto program = std::shared_ptr<GLuint>(new GLuint(glCreateProgram()), del);

	programlist_[shaders] = program;

	for (const auto &shader : shaders) {
		glAttachShader(*program, *shader);
	}

	glLinkProgram(*program);

	GLint status = GL_FALSE;
	glGetProgramiv(*program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		GLint length;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &length);
		std::string log(length, ' ');
		glGetProgramInfoLog(*program, length, &length, &log[0]);

		std::string err("failed to link program: \n");
		throw std::runtime_error(err + log);
	}

	return program;
}

Shader::Shader(const std::map<GLenum, std::string> &shaders) {
	for (const auto &it : shaders) {
		shaders_.insert(Compile(it.first, it.second));
	}

	program_ = Link(shaders_);
}
