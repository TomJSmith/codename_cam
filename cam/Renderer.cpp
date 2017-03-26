#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

#include "Entity.h"
#include "Shader.h"

GLFWwindow *Renderer::window_ = nullptr;

GLFWwindow* Renderer::getWindow()
{
	return window_;
}
// Only initialize GLFW once for the whole application.
// TODO This may need to be called once per thread
static void InitializeGLFW()
{
	static bool initialized = glfwInit() == GLFW_TRUE;
	if (!initialized) throw std::runtime_error("unable to initialize glfw");
}

// InitializeGLEW() must be called after glfwMakeContextCurrent() is called on
// a window.
// TODO this should be called every time a window gets created,
// but we're only doing that once so this is okay
static void InitializeGLEW()
{
	static bool initialized = glewInit() == GLEW_OK;
	if (!initialized) throw std::runtime_error("unable to initialize glew");
}

void Renderer::Initialize()
{
	InitializeGLFW();

	window_ = glfwCreateWindow(1024, 1024, "Title", nullptr, nullptr);

	if (!window_) throw std::runtime_error("unable to create OpenGL window"); // TODO specific exception here?
	glfwMakeContextCurrent(window_);

	InitializeGLEW();
}

void Renderer::Render(Entity &entity)
{
	std::vector<MeshData> data;
	mat4 cam(1.0f);
	Events::Render e {data, cam};

	entity.BroadcastEvent(e);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto view = glm::inverse(mat4(1.0f));
	auto perspective = glm::perspective(45.0f, 1.0f, 0.1f, 400.0f);
	auto vp = perspective * view;

	glEnable(GL_DEPTH_TEST);

	for (auto &d : e.data) {
		d.shader->Setup(d, vp);

		glBindVertexArray(d.vao);
		glDrawArrays(d.type, 0, d.count);
	}

	glEnable(0);
	glfwSwapBuffers(window_);
}