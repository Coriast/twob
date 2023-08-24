#include <twob.h>

using namespace twob;

enum Game
{
	meu_cubo
};

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

// Cluster é para ser criado com um Shader por default
// Cluster é para ser criado com um Material por default
// Esse material vai ser passado para o shader
Cluster* bunch_of_stuff;

void startup()
{
	// criar próprio framebuffer de qualquer tamanho.

	// O cluster deve ser iniciado dentro do Startup pq o construtor dele utiliza o renderer especificado.
	bunch_of_stuff = new Cluster();

	bunch_of_stuff->insert_model(Polygon::CUBE, Game::meu_cubo);

	// carregar os objetos que vão estar na cena
}

void render()
{
	/*
		-> Até o momento a lógica é de que a DrawCall mantém as informações
			do Shader que está sendo utilizado, as informações de Material para passar para ele,
			o tamanho da viewport (buffer) da API, 
			e é a ponte entre o nosso modelo e a renderização do programa.

		-> O Model é o nosso compilado de Meshes, Textures e matrix de transformações no mundo.
	*/

	// renderizar os objetos um por um por enquanto
	
	{

		//bunch_of_stuff.transform(vec3(0.0f, 0.0f, 0.0f), Game::meu_cubo);

		bunch_of_stuff->render();	
	}

	// Batching no futuro
}