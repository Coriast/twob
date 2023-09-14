#include <twob_cluster.h>
#include "twob_canvas.h"
#include "internal/twob_internal.h"

using namespace twob;

Cluster::Cluster()
{
	has_own_shader = false;

	c_shader = nullptr;
	c_material = nullptr;
}

void Cluster::init()
{
	if (c_shader == nullptr)
		c_shader = Internal::app_renderer()->create_shader();
	if (c_material == nullptr)
		c_material = Internal::app_renderer()->create_material();

	c_shader->use();
	c_shader->set_projection();
}

void Cluster::update()
{
	if (App::was_resized())
		c_shader->set_projection();
}

void Cluster::render()
{
	c_shader->use();
	Internal::app_renderer()->render(*this);
}

void Cluster::load_shader(cstr vertex_source, cstr fragment_source)
{
	c_shader = Internal::app_renderer()->create_shader(vertex_source, fragment_source);
}

void Cluster::load_material()
{

}

Model* Cluster::load_model(cstr id, ModelType type, cstr folder_path)
{
	Model* model = Internal::app_renderer()->create_model(type, folder_path);

	c_models[id] = model;
	return model;
}

Model* Cluster::load_primitive(cstr id, Polygon polygon)
{
	Model* model = Internal::app_renderer()->create_primitive(polygon);

	c_models[id] = model;
	return model;
}

const Model& Cluster::get_model(cstr name)
{
	return *c_models[name];
}

void Cluster::translate(cstr id, vec3 translate)
{
	c_models[id]->translate(translate);
}


