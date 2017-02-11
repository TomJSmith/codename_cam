#pragma once

#include "System.h"

#include <map>
#include <memory>
#include <string>
#include <tuple>

#include "Util.h"

class Shader
{
public:
	static Shader &Load(const std::string &vertex, const std::string &fragment);

	// disable copy and assignment so we only delete program once
	Shader(Shader &other) = delete;
	Shader &operator=(Shader &other) = delete;

	// allow move construction so we can store these in a map
	Shader(Shader &&other);

	GLuint Program();

	Shader();
	~Shader();
private:
	Shader(const std::string &vertex, const std::string &fragment);

	GLuint program_;

	static std::map<std::pair<std::string, std::string>, Shader> shaders_;
};

