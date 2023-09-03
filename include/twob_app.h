#pragma once
#include <twob_aliases.h>
#include <twob_graphics.h>
#include <glm/vec2.hpp>

namespace twob
{
	using EventFn = Fn<void>;

	enum Flags
	{
		Fullscreen,
		Resizable
	};

	class Config
	{
	public:
		cstr name = "Default";

		int width = 800;
		int height = 600;

		RendererType renderer_type = RendererType::OpenGL;

		EventFn on_startup = nullptr;

		EventFn on_update = nullptr;
		
		EventFn on_render = nullptr;
	
		EventFn on_shutdown = nullptr;
	
	};

	namespace App
	{
		void run(const Config* config);

		bool is_running();

		bool was_resized(bool close_resize_cycle = false);

		Config* config();

		void set_flag(Flags flag, bool state);

		bool get_flag(Flags flag);

		void set_fullscreen();

		void set_resizable();

		void resize(int width, int height);
	}
}
