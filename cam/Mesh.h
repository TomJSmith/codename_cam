#pragma once

#include "OpenGL.h"

#include <vector>

#include "Component.h"
#include "Shader.h"
#include "Transform.h"

class Mesh : public Component {
public:
	// Structures holding mesh data necessary for rendering, and
	// events fired by the renderer to retrieve mesh data from components
	struct Data { GLuint vao; GLuint shader; mat4 model; };
	struct GetMeshDataEvent { std::vector<Data> &data; };

	Mesh(Shader &shader, GLuint nvertices, GLfloat **vertices, GLfloat **colours);

	void GetMeshData(GetMeshDataEvent event);
protected:
	void RegisterHandlers() override;

private:
	GLfloat **vertices_;
	GLfloat **colours_;

	GLuint vao_;

	Shader &shader_;

	mat4 ModelMatrix();
};
