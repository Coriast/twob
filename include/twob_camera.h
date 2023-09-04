#pragma once
#include <twob_time.h>
#include <twob_input.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace glm;

namespace twob
{

	enum Direction
	{
		FORWARD,
		BACKWARD,
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

	struct MouseHandler
	{
		double x, y;
		bool first_time = true;
	};

	class Camera
	{
	private:
		vec3 position, front, up, right, world_up;

		float yaw, pitch;

		float movement_speed;
		float mouse_sensitivity;

		bool hide_mouse;
		MouseHandler mouse_hand;
		MouseHandler last_mouse;

	public:

		Camera();

		void update();

		mat4 view_matrix();

		void process_keyboard(Direction direct);

		void process_mouse();

		void update_vectors();

		void swap_mouse();
	};
}