#include "Entity.h"
#include "Mesh.h"
#include "Physics.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "ScriptComponent.h"
#include "Shader.h"
#include "Vehicle.h"

#include <iostream>

int main() {
	Renderer::Initialize();
	Renderer renderer;
	Physics physics;
	Entity root;

#ifdef DEBUG
	root.GetEvents().RegisterEventHandler([&physics] (Renderer::GetMeshDataEvent e) {
		auto d = physics.GetDebugMeshData();
		e.data.insert(e.data.end(), d.begin(), d.end());
	});
#endif

	GLfloat vertices[][3] = {
			{-.5, -.5, -.5}, {-.5, .5, -.5}, {.5, .5, -.5},
			{-.5, -.5, -.5}, {.5, .5, -.5}, {.5, -.5, -.5},

			{-.5, -.5, -.5}, {-.5, .5, .5}, {-.5, .5, -.5},
			{-.5, -.5, -.5}, {-.5, -.5, .5}, {-.5, .5, .5},

			{.5, -.5, -.5}, {.5, .5, .5}, {.5, .5, -.5},
			{.5, -.5, -.5}, {.5, -.5, .5}, {.5, .5, .5},

			{-.5, -.5, .5}, {.5, .5, .5}, {-.5, .5, .5},
			{-.5, -.5, .5}, {.5, -.5, .5}, {.5, .5, .5},

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

	GLfloat planeverts[][3] = {
		{0, -10, -10},
		{0, 10, -10},
		{0, -10, 10},
		{0, -10, 10},
		{0, 10, -10},
		{0, 10, 10}
	};

	GLfloat planecolours[][3] = {
		{0.5f, 0.5f, 0.5f},
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
	};

	Entity plane(&root);
	std::shared_ptr<Component> planemesh(new Mesh(Shader::Load("passthrough.vert", "passthrough.frag"), 6, (GLfloat **)planeverts, (GLfloat **)planecolours, GL_TRIANGLES));
	std::shared_ptr<Component> planebody(new RigidBody(physics, *physics.GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f), PxPlaneGeometry(), PxTransform(0.0f, -1.0f, 0.0f, PxQuat(3.14159f / 2.0f, PxVec3(0.0f, 0.0f, 1.0f))), false));
	plane.AddComponent(std::move(planemesh));
	plane.AddComponent(std::move(planebody));

	Entity vehicle(&root);
	std::shared_ptr<Component> mesh(new Mesh(Shader::Load("passthrough.vert", "passthrough.frag"), 36, (GLfloat **)vertices, (GLfloat **)colours, GL_TRIANGLES));
	std::shared_ptr<Component> v(new ScriptComponent("vehicle", physics));
	vehicle.AddComponent(std::move(mesh));
	vehicle.AddComponent(std::move(v));

	auto lastTime = timer::now();

	while (true) {
		auto currentTime = timer::now();
		auto dt = seconds(currentTime - lastTime);
		lastTime = currentTime;

		physics.Update(dt);
		root.Update(dt);
		renderer.Render(root);
	}
}
