#include <twob.h>

using namespace twob;

void startup();
void update();
void render();

int main()
{
	Config config;
	config.name = "twob";
	config.width = 1080;
	config.height = 720;
	config.renderer_type = RendererType::OpenGL;

	config.on_startup = startup;
	config.on_update = update;
	config.on_render = render;

	App::run(&config);
}

// You can create as many clusters as you need
Cluster def;

void startup()
{
	def.init();

	def.load_primitive("cubo1", CUBE)->meshes[0]->set_tex(Path::get_absolute_path("brick_wall.jpg").c_str());
	def.translate("cubo1", vec3(0.0f, 0.0f, -3.0f));

	def.load_primitive("cubo2", CUBE)->meshes[0]->set_tex(Path::get_absolute_path("wall.jpg").c_str());

	Model* bunny = def.load_model("bunny", ModelType::wavefront, Path::get_absolute_path("stanford_bunny").c_str());
	bunny->meshes[0]->set_color(Color::red());
	def.translate("bunny", vec3(1.0f, 1.0f, -2.0f));
}

void update()
{
	def.update();

	if (Input::KeyPressed[GLFW_KEY_G].pressed)
		std::cout << "g pressed  ";
}

void render()
{
	{

		def.render();
	}

}