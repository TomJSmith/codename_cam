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
#include "Text.h"
#include "Vehicle.h"
/*Not sure where to put this afterwards just for testing*/
#include "drawAiPath.h"
#include "NavNode.h"

int main() {
	try {
		Renderer::Initialize();
		Text::Initialize();
		Renderer renderer;
		Physics physics;
		auto root = Entity::Create();

#ifdef DEBUG
		std::function<void(Events::Render)> handler = [&physics](Events::Render e) {
			auto d = physics.GetDebugMeshData();
			e.data.insert(e.data.end(), d.begin(), d.end());
		};

		root->RegisterEventHandler(&handler);
#endif // #ifdef DEBUG


		Audio audio;
		audio.initAudio();
		ALuint source;
		alGenSources(1, &source);
		source = audio.sourceSetup(source, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f), true);

		{
			std::shared_ptr<Component> menu(std::make_shared<ScriptComponent>("main_menu", physics));
			auto e = Entity::Create(root.get()).lock();
			e->AddComponent(std::move(menu));
		}
	
		auto lastTime = timer::now();

		while (!glfwWindowShouldClose(renderer.getWindow())) {
			auto currentTime = timer::now();
			auto dt = seconds(currentTime - lastTime);
			lastTime = currentTime;

			root->Update(dt);
			renderer.Render(*root);

			physics.Update(dt);
			audio.playSounds(*root);
			audio.playAudio(5, source, 5, false, glm::vec3(0.0f,0.0f,0.0f));
			Entity::DeleteDestroyed();

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
