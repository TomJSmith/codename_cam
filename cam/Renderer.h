#pragma once

#include "System.h"

#include <vector>

#include "Transform.h"

class Entity;
class Shader;

class Renderer
{
public:
	void Render(Entity &entity);

	static void Initialize();

	struct MeshData {
		GLuint vao;
		GLuint count;
		GLenum type;
		mat4 modelMatrix;
		Shader *shader;
	};

	GLFWwindow* getWindow();
private:
	static GLFWwindow *window_;
};
