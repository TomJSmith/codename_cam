#include "Shader.h"

#include "OpenGL.h"
#include "Util.h"

std::map<std::pair<std::string, std::string>, Shader> Shader::shaders_;

static GLuint CompileShader(GLenum type, std::string &source)
{
	GLuint shader = glCreateShader(type);
	const GLchar *src = source.c_str();
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) throw std::runtime_error("failed to compile shader"); // TODO real error here

	return shader;
}

static GLuint LinkProgram(GLuint vertex, GLuint fragment)
{
	GLuint program = glCreateProgram();

	if (vertex) glAttachShader(program, vertex);
	if (fragment) glAttachShader(program, fragment);

	glLinkProgram(program);

	GLint status;
	glGetShaderiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) throw std::runtime_error("failed to compile shader"); // TODO real error here

	return program;
}

Shader &Shader::Load(const std::string &vertex, const std::string &fragment)
{
	auto key = std::make_pair(vertex, fragment);
	if (shaders_.count(key) == 0) {
		shaders_.emplace(std::make_pair(key, Shader(vertex, fragment)));
	}

	return shaders_[key];
}

Shader::Shader() : program_(0) {}
Shader::Shader(Shader &&other) {std::swap(program_, other.program_);}

Shader::Shader(const std::string &vertex, const std::string &fragment)
{
	GLuint v = CompileShader(GL_VERTEX_SHADER, Util::ReadFile(vertex));
	GLuint f = CompileShader(GL_FRAGMENT_SHADER, Util::ReadFile(fragment));

	program_ = LinkProgram(v, f);

	glDeleteShader(v);
	glDeleteShader(f);
}

Shader::~Shader()
{
	glDeleteProgram(program_);
}

GLuint Shader::Program()
{
	return program_;
}
