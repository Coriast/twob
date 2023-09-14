#include <twob_camera.h>
#include <twob_canvas.h>

using namespace glm;
using namespace twob;

Camera::Camera()
{
	position = vec3(1.0f, 1.0f, 5.0f);
	front = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	world_up = up;
	right = normalize(cross(front, world_up));

	yaw = -90.0f;
	pitch = 0.0f;
	movement_speed = 2.5f;
	mouse_sensitivity = 2.5f;

	hide_mouse = false;

	int scr_width, scr_height;
	Canvas::get_screen_size(&scr_width, &scr_height);
	last_mouse.x = scr_width / 2.0f;
	last_mouse.y = scr_height / 2.0f;
}

void Camera::update()
{
	if (Input::KeyHold[GLFW_KEY_W])
		process_keyboard(FORWARD);

	if (Input::KeyHold[GLFW_KEY_D])
		process_keyboard(RIGHT);

	if (Input::KeyHold[GLFW_KEY_A])
		process_keyboard(LEFT);

	if (Input::KeyHold[GLFW_KEY_S])
		process_keyboard(BACKWARD);

	if (Input::KeyHold[GLFW_KEY_LEFT_SHIFT])
		process_keyboard(UP);

	if (Input::KeyHold[GLFW_KEY_LEFT_CONTROL])
		process_keyboard(DOWN);

	if (Input::KeyPressed[GLFW_MOUSE_BUTTON_LEFT].pressed)
		swap_mouse();

	if (hide_mouse)
		process_mouse();
}

void Camera::swap_mouse()
{
	Canvas::mouse_cursor(hide_mouse);
	hide_mouse = !hide_mouse;
}

mat4 Camera::view_matrix()
{
	return lookAt(position, position + front, up);
}

void Camera::process_keyboard(Direction direct)
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
	if (direct == Direction::UP)
	{
		position += up * velocity;
	}
	if (direct == Direction::DOWN)
	{
		position -= up * velocity;
	}
}

void Camera::process_mouse()
{
	Canvas::get_cursor(&mouse_hand.x, &mouse_hand.y);

	if (mouse_hand.first_time)
	{
		last_mouse.x = mouse_hand.x;
		last_mouse.y = mouse_hand.y;
		mouse_hand.first_time = false;
	}

	float x_offset = mouse_hand.x - last_mouse.x;
	float y_offset = last_mouse.y - mouse_hand.y;

	last_mouse.x = mouse_hand.x;
	last_mouse.y = mouse_hand.y;

	x_offset *= mouse_sensitivity * Time::delta();
	y_offset *= mouse_sensitivity * Time::delta();

	yaw += x_offset;
	pitch += y_offset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	update_vectors();
}

void Camera::update_vectors()
{
	vec3 _front;
	_front.x = cos(radians(yaw)) * cos(radians(pitch));
	_front.y = sin(radians(pitch));
	_front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = normalize(_front);

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
}