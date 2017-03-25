#pragma once

#include "System.h"

#include <functional>
#include <memory>
#include <vector>

#include "Component.h"
#include "Events.h"
#include "Transform.h"
#include "Texture.h"

class Shader;

class Mesh : public Component {
public:
	Mesh(std::unique_ptr<Shader> shader,
		 const char* objFileName,
		 glm::vec3 colour,
		 glm::vec3 scale,
		 GLuint type);

	void GetMeshData(Events::Render event);
	void Destroy() override;

protected:
	void RegisterHandlers() override;

private:
	GLuint vao_;
	GLuint type_;
	GLuint count_;

	std::unique_ptr<Shader> shader_;

	std::function<void(Events::Render)> handler_;
};
