#include "Audio.h" //has to be at the top for some reason
#include "Controller.h"
#include "Entity.h"
#include "Mesh.h"
#include "Physics.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "ScriptComponent.h"
#include "Shader.h"
#include "Vehicle.h"



int main() {
	Renderer::Initialize();
	Renderer renderer;
	Physics physics;
	Entity root;

#ifdef DEBUG
	root.GetEvents().RegisterEventHandler([&physics](Renderer::GetMeshDataEvent e) {
		auto d = physics.GetDebugMeshData();
		e.data.insert(e.data.end(), d.begin(), d.end());
	});
#endif

	Audio audio;
	audio.initAudio();
	audio.playAudio(4); //1,2,3,4 for Audio atm can play more than one at a time
	
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

	bool soundT = true;
	while (!glfwWindowShouldClose(renderer.getWindow())) {
		auto currentTime = timer::now();
		auto dt = seconds(currentTime - lastTime);
		lastTime = currentTime;
	
		//audio.playAudio(1);
		physics.Update(dt);
		root.Update(dt);
		renderer.Render(root);
	
		glfwPollEvents();

	}
	
	return 0;
}
