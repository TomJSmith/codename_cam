#ifndef __MESH_H_
#define __MESH_H_

#include "OpenGL.h"

#include <vector>

#include "Component.h"
#include "Shader.h"
#include "Transform.h"

class Mesh : public Component {
public:
	struct Data { GLuint vao; GLuint shader; mat4 model; };
	struct GetMeshDataEvent { std::vector<Data> &data; };

	Mesh(Shader &shader, GLuint nvertices, GLfloat **vertices, GLfloat **colours);
	void GetMeshData(GetMeshDataEvent event);
	mat4 ModelMatrix();

protected:
	void RegisterHandlers() override;

private:
	GLfloat **vertices_;
	GLfloat **colours_;

	GLuint vao_;

	Shader &shader_;
};

#endif // __MESH_H_
