#include "twob_canvas.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <twob_input.h>
#include <internal/twob_internal.h>

using namespace twob;

namespace
{
	GLFWwindow* glfw_window;
	GLFWmonitor* glfw_monitor;
	const GLFWvidmode* glfw_mode;

	void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mode)
	{
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				Input::KeyHold[key] = true;

				if (Input::KeyPressed[key].released)
				{
					Input::KeyPressed[key].pressed = true;
					Input::KeyPressed[key].released = false;
				}
			}
			else if (action == GLFW_RELEASE)
			{
				Input::KeyHold[key] = false;
				Input::KeyPressed[key].pressed = false;
				Input::KeyPressed[key].released = true;
			}
		}
	}

	void mouse_click_callback(GLFWwindow* window, int key, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			Input::KeyHold[key] = true;

			if (Input::KeyPressed[key].released)
			{
				Input::KeyPressed[key].pressed = true;
				Input::KeyPressed[key].released = false;
			}
		}
		else if (action == GLFW_RELEASE)
		{
			Input::KeyHold[key] = false;
			Input::KeyPressed[key].pressed = false;
			Input::KeyPressed[key].released = true;
		}
	}

	void window_size_callback(GLFWwindow* glfw_window, int width, int height)
	{
		if (width == 0 || height == 0)
			return;

		App::resize(width, height);
		Internal::app_renderer()->set_viewport();
	}
}


void Canvas::init()
{
	glfwInit();

	switch (App::config()->renderer_type)
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

	glfw_monitor = glfwGetPrimaryMonitor();
	glfw_mode = glfwGetVideoMode(glfw_monitor);

	glfw_window = glfwCreateWindow(App::config()->width, App::config()->height, App::config()->name, nullptr, nullptr);
	glfwMakeContextCurrent(glfw_window);
	glfwSetKeyCallback(glfw_window, key_callback);
	glfwSetMouseButtonCallback(glfw_window, mouse_click_callback);
	glfwSetWindowSizeCallback(glfw_window, window_size_callback);

	// glad related
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

}

void Canvas::set_fullscreen(bool state)
{
	if (state)
		glfwSetWindowMonitor(glfw_window, glfw_monitor, 0, 0, glfw_mode->width, glfw_mode->height, glfw_mode->refreshRate);
	else
		glfwSetWindowMonitor(glfw_window, nullptr, 200, 200, App::config()->width, App::config()->height, 0);
}

void Canvas::set_resizable(bool state)
{
}

void Canvas::swapBuffer()
{
	if (App::config()->renderer_type == RendererType::OpenGL)
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

	if (Input::KeyPressed[GLFW_KEY_ESCAPE].pressed)
		Internal::exit_app();

	if (Input::KeyPressed[GLFW_KEY_F11].pressed)
		App::set_fullscreen();


}

void Canvas::close_update()
{
	map<int, CheckRelease>::iterator it = Input::KeyPressed.begin();
	while (it != Input::KeyPressed.end())
	{
		if (it->second.pressed)
		{
			it->second.pressed = false;
		}
		it++;
	}

	App::was_resized(true);
}

double Canvas::app_time()
{
	return glfwGetTime();
}

void Canvas::get_cursor(double* x, double* y)
{
	glfwGetCursorPos(glfw_window, x, y);
}

void Canvas::get_screen_size(int* width, int* height)
{
	glfwGetFramebufferSize(glfw_window, width, height);
}

void Canvas::mouse_cursor(bool enabled)
{
	glfwSetInputMode(glfw_window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}