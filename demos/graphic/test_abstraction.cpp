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
	config.name = "Teste Abstra��o";
	config.width = 720;
	config.height = 720;
	config.renderer_type = RendererType::OpenGL;

	config.on_startup = startup;
	config.on_render = render;

	App::run(&config);
}

// Cluster � para ser criado com um Shader por default
// Cluster � para ser criado com um Material por default
// Esse material vai ser passado para o shader
Cluster* bunch_of_stuff;

void startup()
{
	// criar pr�prio framebuffer de qualquer tamanho.

	// O cluster deve ser iniciado dentro do Startup pq o construtor dele utiliza o renderer especificado.
	bunch_of_stuff = new Cluster();

	bunch_of_stuff->insert_model(Polygon::CUBE, Game::meu_cubo);

	// carregar os objetos que v�o estar na cena
}

void render()
{
	/*
		-> At� o momento a l�gica � de que a DrawCall mant�m as informa��es
			do Shader que est� sendo utilizado, as informa��es de Material para passar para ele,
			o tamanho da viewport (buffer) da API, 
			e � a ponte entre o nosso modelo e a renderiza��o do programa.

		-> O Model � o nosso compilado de Meshes, Textures e matrix de transforma��es no mundo.
	*/

	// renderizar os objetos um por um por enquanto
	
	{

		//bunch_of_stuff.transform(vec3(0.0f, 0.0f, 0.0f), Game::meu_cubo);

		bunch_of_stuff->render();	
	}

	// Batching no futuro
}