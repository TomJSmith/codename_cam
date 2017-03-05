#pragma once

#include "System.h"

#include <functional>
#include <vector>

#include "Component.h"
#include "Events.h"
#include "Transform.h"

class Shader;

class Mesh : public Component {
public:
	Mesh(Shader &shader,
		 const char* objFileName,
		 glm::vec3 colour,
		 float scale,
		 GLuint type);
	~Mesh();

	void GetMeshData(Events::Render event);

protected:
	void RegisterHandlers() override;

private:
	GLuint vao_;
	GLuint type_;
	GLuint count_;

	Shader &shader_;

	std::function<void(Events::Render)> handler_;
};
