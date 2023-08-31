#include <twob.h>

using namespace twob;

void startup();
void render();

int main()
{
	Config config;
	config.name = "Teste Abstração";
	config.width = 720;
	config.height = 720;
	config.renderer_type = RendererType::OpenGL;

	config.on_startup = startup;
	config.on_render = render;

	App::run(&config);
}

// You can create as many clusters as you need
Cluster bunch_of_stuff;

void startup()
{
	bunch_of_stuff.init();

	bunch_of_stuff.load_model(Polygon::CUBE, "cubo", Path::get_absolute_path("wall.jpg").c_str());
	
}

void render()
{
	{
		//bunch_of_stuff.translate(vec3(0.0f, 0.0f, 0.0f), Objects::cube);

		bunch_of_stuff.render();	
	}

}