#pragma once
#include <twob_app.h>
#include <twob_graphics.h>
#include <glm/vec2.hpp>

namespace twob
{

	namespace Canvas
	{
		// Create the GLFW window and context based on RendererType
		void init(Config& config);

		void set_fullscreen(bool state);

		void set_resizable(bool state);

		void swapBuffer();

		// Terminate the GLFW stuff
		void shutdown();

		// To update all Input States (Mouse, Keyboard, Gamepads)
		void update();

		void get_size(int* width, int* height);

	}
}