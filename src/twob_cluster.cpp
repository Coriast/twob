#include <twob_cluster.h>
#include "twob_canvas.h"
#include "internal/twob_internal.h"

using namespace twob;

Cluster::Cluster()
{
	has_own_shader = false;

	c_viewport = new Viewport(0, 0, 0, 0);
	c_shader = nullptr;
	c_material = nullptr;
	c_camera = nullptr;
}

void Cluster::init()
{
	if (c_shader == nullptr)
		c_shader = Internal::app_renderer()->create_shader();
	if (c_material == nullptr)
		c_material = Internal::app_renderer()->create_material();
	if (c_camera == nullptr)
		c_camera = Internal::app_renderer()->create_camera();

}

void Cluster::render()
{
	{ 
		int w, h;
		Canvas::get_size(&w, &h);
		*c_viewport = { 0, 0, w, h };
	}
	
	Internal::app_renderer()->render(*this);
}

void Cluster::load_shader(cstr vertex_source, cstr fragment_source)
{
	c_shader = Internal::app_renderer()->create_shader(vertex_source, fragment_source);
}

void Cluster::load_material()
{

}

void Cluster::load_model(cstr file_path, ModelId m)
{
}

void Cluster::load_model(Polygon polygon, ModelId m)
{
	Model* model = Internal::app_renderer()->create_model(polygon);
	c_models[m] = model;
}
