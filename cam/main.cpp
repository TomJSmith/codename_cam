#include "Audio.h" //has to be at the top for some reason
#include "Controller.h"

#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

#include "Camera.h"
#include "Entity.h"
#include "Image.h"
#include "Mesh.h"
#include "ModelShader.h"
#include "NavMesh.h"
#include "Physics.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "ScriptComponent.h"
#include "Shader.h"
#include "Vehicle.h"
/*Not sure where to put this afterwards just for testing*/
#include "drawAiPath.h"
#include "NavNode.h"

int main() {
	try {
		Renderer::Initialize();
		Renderer renderer;
		Physics physics;
		auto root = Entity::Create();


#ifdef DEBUG

		std::function<void(Events::Render)> handler = [&physics](Events::Render e) {
			auto d = physics.GetDebugMeshData();
			e.data.insert(e.data.end(), d.begin(), d.end());
		};

		root->RegisterEventHandler(&handler);

		//NavMesh testMesh = NavMesh("test_nav.fbx", glm::vec3(2.0f, 2.0f, 2.0f));
		//testMesh.nodeGraph[0].isInside(0, 0);
		//testMesh.nodeGraph[0].isInside(0.9f, 0);
		//testMesh.nodeGraph[0].isInside(1, 0);
		//testMesh.nodeGraph[0].isInside(1.1f, 0);
		//testMesh.nodeGraph[0].isInside(-1.1f, 0);
		//testMesh.nodeGraph[0].isInside(0, 1);
		//testMesh.nodeGraph[0].isInside(0, -1.1f);
		//drawAiPath drawPath;

		//std::function<void(Events::Render)> pathHandle = [&drawPath](Events::Render b) {
		//	auto d = drawPath.GetDebugMeshData();
		//	b.data.insert(b.data.end(), d.begin(), d.end());
		//};
		//root->RegisterEventHandler(&pathHandle);

#endif // #ifdef DEBUG


		Audio audio;

	//	audio.initAudio();
		//audio.playAudio(4); //1,2,3,4 for Audio atm can play more than one at a time



		{
			auto plane = Entity::Create(root.get()).lock();
			std::shared_ptr<Component> planemesh(new Mesh(std::unique_ptr<Shader>(new ModelShader("test.jpg")), "map_mesh.fbx", vec3(0.2, 0.4, 0.2), vec3(2.0f, 2.0f, 2.0f), GL_TRIANGLES));
			std::shared_ptr<Component> planebody(new RigidBody(physics, *physics.GetPhysics()->createMaterial(1.0f, 1.0f, 1.0f), "map_mesh.fbx", 2.0f, false));
			plane->AddComponent(std::move(planemesh));
			plane->AddComponent(std::move(planebody));

			
			auto aiVehicle = Entity::Create(root.get()).lock();
			std::shared_ptr<Component> aiMesh(new Mesh(std::unique_ptr<Shader>(new ModelShader("runner_texture.jpg")), "runner_mesh.fbx", vec3(1.0, 0.84, 0.0), vec3(4.427f, 2.426f, 12.935f), GL_TRIANGLES));
			std::shared_ptr<Component> aiV(new ScriptComponent("chaser_ai", physics));
			aiVehicle->AddComponent(std::move(aiMesh));
			aiVehicle->AddComponent(std::move(aiV));
			aiVehicle->AddComponent(std::make_unique<ScriptComponent>("chaser", physics));
			
			
			auto vehicle = Entity::Create(root.get()).lock();
			std::shared_ptr<Component> mesh(new Mesh(std::unique_ptr<Shader>(new ModelShader("runner_texture.jpg")), "runner_mesh.fbx", vec3(0.1, 0.1, 0.6), vec3(4.427f, 2.426f, 12.935f), GL_TRIANGLES));
			std::shared_ptr<Component> v(new ScriptComponent("vehicle", physics));
			std::shared_ptr<Component> c(new ScriptComponent("collision", physics));
			vehicle->AddComponent(std::move(mesh));
			vehicle->AddComponent(std::move(v));
			vehicle->AddComponent(std::move(c));
			//vehicle->AddComponent(std::make_unique<ScriptComponent>("runner", physics));
			
			
			auto aiVehicleRun = Entity::Create(root.get()).lock();
			std::shared_ptr<Component> aiMeshRun(new Mesh(std::unique_ptr<Shader>(new ModelShader("runner_texture.jpg")), "runner_mesh.fbx", vec3(1.0, 0.84, 0.0), vec3(4.427f, 2.426f, 12.935f), GL_TRIANGLES));
			std::shared_ptr<Component> aiVRun(new ScriptComponent("runner_ai", physics));
			aiVehicleRun->AddComponent(std::move(aiMeshRun));
			aiVehicleRun->AddComponent(std::move(aiVRun));
			//aiVehicleRun->AddComponent(std::make_unique<ScriptComponent>("runner", physics));
			

			/*auto smiley = Entity::Create(root.get()).lock();
			std::shared_ptr<Component> image(new Image("runner_texture.jpg"));
			smiley->AddComponent(std::move(image));*/

		}


	
		auto lastTime = timer::now();


		while (!glfwWindowShouldClose(renderer.getWindow())) {
			auto currentTime = timer::now();
			auto dt = seconds(currentTime - lastTime);

			lastTime = currentTime;

			root->Update(dt);
			physics.Update(dt);
			renderer.Render(*root);
			//audio.playAudio(4);
			Entity::DeleteDestroyed();

			glfwPollEvents();

			//std::cout << "frame time: " << dt.count() << " seconds\n";
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
