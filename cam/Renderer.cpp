#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

#include "Entity.h"

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

	glEnable(GL_DEPTH_TEST);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window_ = glfwCreateWindow(640, 480, "Title", nullptr, nullptr);

	if (!window_) throw std::runtime_error("unable to create OpenGL window"); // TODO specific exception here?
	glfwMakeContextCurrent(window_);

	InitializeGLEW();
}

void Renderer::Render(Entity &entity)
{
	std::vector<MeshData> data;
	mat4 cam(1.0f);
	RenderEvent e {data, cam};

	entity.BroadcastEvent(e);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto view = glm::inverse(cam);
	auto perspective = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	auto vp = perspective * view;

	for (auto &d : e.data) {
		auto mvp =  vp * d.modelMatrix;
		auto modelMatrix = d.modelMatrix;

		glUseProgram(d.shader);

		glUniformMatrix4fv(glGetUniformLocation(d.shader, "mvp"), 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(d.shader, "projection"), 1, GL_FALSE, &vp[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(d.shader, "modelview"), 1, GL_FALSE, &modelMatrix[0][0]);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(d.vao);
		glDrawArrays(d.type, 0, d.count);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	glfwSwapBuffers(window_);
}