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
		LEFT
	};

	class Camera
	{
	private:
		vec3 position, front, up, right, world_up;

		float yaw, pitch;

		float movement_speed = 2.5f;
		float mouse_sensitivity = 1.0f;

	public:

		Camera()
		{
			position = vec3(0.0f, 0.0f, 3.0f);
			front = vec3(0.0f, 0.0f, -1.0f);
			up = vec3(0.0f, 1.0f, 0.0f);
			world_up = up;
			right = normalize(cross(front, world_up));
			
			yaw = -90.0f;
			pitch = 0.0f;
			movement_speed = 2.5f;
			mouse_sensitivity = 1.0f;

		}

		void update()
		{
			if (Input::KeyHold[GLFW_KEY_W])
				process_keyboard(FORWARD);

			if (Input::KeyHold[GLFW_KEY_D])
				process_keyboard(RIGHT);

			if (Input::KeyHold[GLFW_KEY_A])
				process_keyboard(LEFT);

			if (Input::KeyHold[GLFW_KEY_S])
				process_keyboard(BACKWARD);
		}

		mat4 view_matrix() 
		{
			return lookAt(position, position + front, up);
		}

		void process_keyboard(Direction direct) 
		{
			float velocity = movement_speed * Time::delta();
			if (direct == Direction::FORWARD)
			{
				position += front * velocity;
			}
			if (direct == Direction::BACKWARD)
			{
				position -= front * velocity;
			}
			if (direct == Direction::LEFT)
			{
				position -= right * velocity;
			}
			if (direct == Direction::RIGHT)
			{
				position += right * velocity;
			}
		}

		void process_mouse(float x_offset, float y_offset) 
		{
			x_offset *= mouse_sensitivity;
			y_offset *= mouse_sensitivity;

			yaw += x_offset;
			pitch += y_offset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			update_vectors();
		}

		void update_vectors() 
		{
			vec3 front;
			front.x = cos(radians(yaw)) * cos(radians(pitch));
			front.y = sin(radians(pitch));
			front.z = sin(radians(yaw)) * cos(radians(pitch));
			front = normalize(front);

			right = normalize(cross(front, world_up));
			up = normalize(cross(right, front));
		}
	};
}