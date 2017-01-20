#include "Entity.h"
#include "Physics.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "ScriptComponent.h"

#include <iostream>

int main() {
	Renderer::Initialize();
	Renderer renderer;
	Physics physics;
	Entity root;

	GLfloat vertices[][3] = {
			{-.5, -.5, -.5}, {-.5, .5, -.5}, {.5, .5, -.5},
			{-.5, -.5, -.5}, {.5, .5, -.5}, {.5, -.5, -.5},

			{-.5, -.5, -.5}, {-.5, .5, .5}, {-.5, .5, -.5},
			{-.5, -.5, -.5}, {-.5, -.5, .5}, {-.5, .5, .5},

			{.5, -.5, -.5}, {.5, .5, .5}, {.5, .5, -.5},
			{.5, -.5, -.5}, {.5, -.5, .5}, {.5, .5, .5},

			{-.5, -.5, .5}, {.5, .5, .5}, {-.5, .5, .5},
			{-.5, -.5, .5}, {.5, -.5, .5}, {.5, .5, .5},

			{ -.5, .5, -.5 },{ .5, .5, .5 },{ -.5, .5, .5 },
			{ -.5, .5, -.5 },{ .5, .5, -.5 },{ .5, .5, .5 },

			{ -.5, -.5, -.5 },{ .5, -.5, .5 },{ -.5, -.5, .5 },
			{ -.5, -.5, -.5 },{ .5, -.5, -.5 },{ .5, -.5, .5 },
	};

	GLfloat colours[][3] = {
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },
		{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },
		{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },
		{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },
	};

	std::unique_ptr<Component> script(new ScriptComponent("component"));
	std::unique_ptr<Component> mesh(new Mesh(Shader::Load("passthrough.vert", "passthrough.frag"), 36, (GLfloat **)vertices, (GLfloat **)colours));
	std::unique_ptr<Component> rigidbody(new RigidBody(physics, *physics.GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f), PxBoxGeometry(.5f, .5f, .5f), PxTransform(0.0f, 0.0f, -2.0f, PxQuat(0, 0, 1, 0))));
	root.AddComponent(std::move(mesh));
	root.AddComponent(std::move(script));
	root.AddComponent(std::move(rigidbody));

	//root.transform().position = vec3(0.0f, 0.0f, -2.0f);

	timer::time_point lastTime = timer::now();

	while (true) {
		timer::time_point currentTime = timer::now();
		//nanoseconds dt = duration_cast<nanoseconds>(currentTime - lastTime);
		auto dt = seconds(currentTime - lastTime);
		lastTime = currentTime;

		physics.Update(dt);
		root.Update(dt);
		renderer.Render(root);
	}
}
