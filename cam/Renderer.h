#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "OpenGL.h"

#include "Entity.h"
#include "Shader.h"

class Renderer
{
public:
	void Render(Entity &entity);

	static void Initialize();
private:
	static GLFWwindow *window_;
};

#endif // __RENDERER_H_