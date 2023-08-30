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

	Renderer*	app_renderer_api;
}

void app_step()
{
	Canvas::update();
	if (app_config.on_update != nullptr)
		app_config.on_update();

	// actually draw
	{
		if (app_config.on_render != nullptr)
			app_config.on_render();
		Canvas::swapBuffer();
	}
}

void app_shutdown()
{
	Canvas::shutdown();
}

void App::run(const Config* config)
{
	app_config = *config;

	Canvas::init(app_config);

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

const Config& App::config()
{
	return app_config;
}

Renderer* Internal::app_renderer()
{
	return app_renderer_api;
}

glm::vec2 App::get_size()
{
	ivec2 size;
	Canvas::get_size(&size.x, &size.y);
	return size;
}

void App::set_flag(Flags flag, bool enabled)
{
	switch (flag)
	{
	case Fullscreen:
		if (app_flags.fullscreen != enabled)
		{
			Canvas::set_fullscreen(enabled);
			app_flags.fullscreen = enabled;
		}
		break;
	case Resizable:
		if (app_flags.resizable != enabled)
		{
			Canvas::set_resizable(enabled);
			app_flags.resizable = enabled;
		}
		break;
	default:
		break;
	}
}


