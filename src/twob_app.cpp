#include "twob_app.h"
#include <twob_canvas.h>
#include "twob_renderer.h"
#include "twob_internal.h"

using namespace twob;

namespace
{
	struct AppFlags
	{
		bool fullscreen	= false;
		bool resizable	= true;
	};
	AppFlags app_flags;

	Config		app_config;
	bool		app_is_running = false;
	bool		app_is_exiting = false;
	bool		app_resized	= false;

	Renderer*	app_renderer_api;
}

void app_step()
{
	// Update of inputs and bla bla bla
	{
		Canvas::update();
		Internal::app_renderer()->camera()->update();

		if (app_config.on_update != nullptr)
			app_config.on_update();
	}
	Canvas::close_update();

	// actually draw
	{
		if (app_config.on_render != nullptr)
			app_config.on_render();
		Canvas::swapBuffer();
	}
	Time::update_delta(Canvas::app_time());
}

void app_shutdown()
{
	Canvas::shutdown();
}

void App::run(const Config* config)
{
	app_config = *config;

	Canvas::init();

	app_is_running = true;
	app_is_exiting = false;

	app_renderer_api = Renderer::instance_renderer(app_config.renderer_type);

	app_renderer_api->init();
	if (app_config.on_startup != nullptr)
		app_config.on_startup();

	while (!app_is_exiting)
		app_step();

	if (app_config.on_shutdown != nullptr)
		app_config.on_shutdown();

	app_shutdown();
}

bool App::is_running()
{
	return app_is_running;
}

Config* App::config()
{
	return &app_config;
}

void App::set_flag(Flags flag, bool state)
{
	switch (flag)
	{
	case Fullscreen:
			Canvas::set_fullscreen(state);
		break;
	case Resizable:
			Canvas::set_resizable(state);
		break;
	default:
		break;
	}
}

void App::set_fullscreen()
{
	app_flags.fullscreen = !app_flags.fullscreen;
	set_flag(Flags::Fullscreen, app_flags.fullscreen);
}

void App::resize(int width, int height)
{
	app_config.width = width;
	app_config.height = height;
	app_resized = true;
}

bool App::was_resized(bool close_resize_cycle)
{
	if (close_resize_cycle)
		app_resized = false;
	return app_resized;
}

bool App::get_flag(Flags flag)
{
	switch (flag)
	{
	case Fullscreen:
		return app_flags.fullscreen;
		break;
	case Resizable:
		return app_flags.resizable;
		break;
	default:
		break;
	}
}

Renderer* Internal::app_renderer()
{
	return app_renderer_api;
}

void Internal::exit_app()
{
	app_is_exiting = true;
}