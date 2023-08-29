#include <twob_cluster.h>
#include "twob_canvas.h"
#include "internal/twob_internal.h"

using namespace twob;

Cluster::Cluster()
{
	has_viewport = false;
	has_own_shader = false;

	viewport = { 0, 0, 0, 0 };
	shader = nullptr;
	model = nullptr;
	material = Internal::app_renderer()->create_material();
}

void Cluster::render()
{
	// Get Viewport size from window size
	{ 
		int w, h;
		Canvas::get_size(&w, &h);
		viewport = { 0, 0, w, h };
	}
	
	// Render the models in this Cluster
	Internal::app_renderer()->render(*this);
}

void Cluster::insert_model(const char* file_path, ModelId m)
{
}

void Cluster::insert_model(Polygon polygon, ModelId m)
{
	Model* model = Internal::app_renderer()->create_model(polygon);
	models[m] = model;
}

HashModel Cluster::get_models()
{
	return models;
}