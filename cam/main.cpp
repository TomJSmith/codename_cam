#include "Audio.h" //has to be at the top for some reason
#include "Controller.h"

#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

#include "Camera.h"
#include "Entity.h"
#include "Mesh.h"
#include "Physics.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "ScriptComponent.h"
#include "Shader.h"
#include "Vehicle.h"



int main() {
	try {
		Renderer::Initialize();
		Renderer renderer;
		Physics physics;
		auto root = Entity::Create(nullptr);

#ifdef DEBUG
		root.GetEvents().RegisterEventHandler([&physics](Renderer::RenderEvent e) {
			auto d = physics.GetDebugMeshData();
			e.data.insert(e.data.end(), d.begin(), d.end());
		});
#endif

		Audio audio;
		audio.initAudio();
		//audio.playAudio(4); //1,2,3,4 for Audio atm can play more than one at a time

		auto plane = Entity::Create(root.get());
		std::shared_ptr<Component> planemesh(new Mesh(Shader::Load("passthrough.vert", "passthrough.frag"), "test_map_mesh.obj", vec3(0.5, 0.5, 0.5), 2.5, GL_TRIANGLES));
		std::shared_ptr<Component> planebody(new RigidBody(physics, *physics.GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f), "test_map_mesh.obj", 2.5f));
		plane->AddComponent(std::move(planemesh));
		plane->AddComponent(std::move(planebody));

		auto vehicle = Entity::Create(root.get());
		std::shared_ptr<Component> mesh(new Mesh(Shader::Load("passthrough.vert", "passthrough.frag"), "runner_mesh.fbx", vec3(0, 0, 1), 2.5, GL_TRIANGLES));
		std::shared_ptr<Component> v(new ScriptComponent("vehicle", physics));
		vehicle->AddComponent(std::move(mesh));
		vehicle->AddComponent(std::move(v));

		auto camera = Entity::Create(vehicle.get());
		std::shared_ptr<Component> cam(new Camera);
		std::shared_ptr<Component> ctrl(new ScriptComponent("camera_control", physics));
		camera->AddComponent(std::move(cam));
		camera->AddComponent(std::move(ctrl));

		/*
		Entity chaser(&root);
		std::shared_ptr<Component> ai(new ScriptComponent("chaser_ai", physics));
		chaser.AddComponent(std::move(ai));
		*/
		auto lastTime = timer::now();

		bool soundT = true;
		while (!glfwWindowShouldClose(renderer.getWindow())) {
			auto currentTime = timer::now();
			auto dt = seconds(currentTime - lastTime);
			lastTime = currentTime;

			//audio.playAudio(1);
			physics.Update(dt);
			root->Update(dt);
			renderer.Render(*root);

			glfwPollEvents();

		}

		return 0;
	}
	catch (const std::runtime_error &e) {
		std::cout << "Fatal error: " << e.what() << "\n";
		return 1;
	}
	catch (...) {
		std::cout << "Unrecognized fatal error\n";
		return 1;
	}
}
