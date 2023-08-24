#include "twob_canvas.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace twob;

namespace
{
	GLFWwindow* glfw_window;
}

void Canvas::init(Config& config)
{
	glfwInit();

	switch (config.renderer_type)
	{
	case RendererType::OpenGL:
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
		break;
	case RendererType::Vulkan:
		{ }
		break;
	}

	glfw_window = glfwCreateWindow(config.width, config.height, config.name, nullptr, nullptr);
	glfwMakeContextCurrent(glfw_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Canvas::set_fullscreen(bool state)
{
}

void Canvas::set_resizable(bool state)
{
}

void Canvas::swapBuffer()
{
	if (App::config().renderer_type == RendererType::OpenGL)
		glfwSwapBuffers(glfw_window);
}

void Canvas::shutdown()
{
	glfwDestroyWindow(glfw_window);
	glfwTerminate();
}

void Canvas::update()
{
	glfwPollEvents();
}

void Canvas::get_size(int* width, int* height)
{
	glfwGetWindowSize(glfw_window, width, height);
}

