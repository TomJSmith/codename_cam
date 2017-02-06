#pragma once

#include "OpenGL.h"

#include "Entity.h"
#include "Shader.h"

class Renderer
{
public:
	void Render(Entity &entity);

	static void Initialize();
	GLFWwindow* getWindow();
private:
	static GLFWwindow *window_;
};