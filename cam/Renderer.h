#pragma once

#include <vector>

#include "OpenGL.h"

#include "Transform.h"

class Entity;

class Renderer
{
public:
	void Render(Entity &entity);

	static void Initialize();

	struct MeshData {
		GLuint vao;
		GLuint shader;
		GLuint count;
		GLenum type;
		mat4 modelMatrix;
	};

	struct GetMeshDataEvent {
		std::vector<MeshData> &data;
	};

	GLFWwindow* getWindow();
private:
	static GLFWwindow *window_;
};
