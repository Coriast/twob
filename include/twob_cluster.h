#pragma once
#include <twob_graphics.h>
#include <glm/vec3.hpp>
#include <unordered_map>

using namespace glm;

namespace twob
{
	typedef std::unordered_map<std::string, Model*> MapModel;
	/* 
	* Some kind of All-in- one class. The Cluster's principle is to:
	* -> Be a 3D models basket, where you can push all the models that
	*    will use the same Shader + Material configurations.
	* 
	* -> That way you can call just one render for each bunch of models 
	*	 that'll use the same configurations.
	* 
	* -> Probably camera will be set apart of the rendering specific architecture:
	*    Camera now is an unique object.
	*	 Viewport also is distatched from the Cluster, don't make sense to keep it bounded.
	*/
	class Cluster
	{
	public:
		Cluster();

		void init();

		void update();

		void render();

		// load content 
		void load_shader(cstr vertex_source, cstr fragment_source);
		void load_material();

		Model* load_model(cstr id, ModelType type, cstr folder_path);

		Model* load_primitive(cstr id, Polygon shape);

		const Model& get_model(cstr id);

		void translate(cstr id, vec3 translate);
		void rotate(cstr id, vec3 axis, float angle);
		void scale(cstr id, vec3 scale);

		// Getters
		const MapModel&	models()	{ return c_models; }
		Shader*		shader()	{ return c_shader; }
		Material*	material()	{ return c_material; }



	private:
		bool has_own_shader;

		Shader*		c_shader;
		Material*	c_material;

		MapModel c_models;
	};
}