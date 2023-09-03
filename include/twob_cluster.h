#pragma once
#include <twob_graphics.h>
#include <glm/vec3.hpp>
#include <unordered_map>

using namespace glm;
using namespace std;

namespace twob
{
	typedef unordered_map<string, Model*> MapModel;
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

		// load content 
		void load_shader(cstr vertex_source, cstr fragment_source);
		void load_material();

		void load_model(cstr file_path, cstr name);
		void load_model(Polygon shape, cstr name, cstr texture_path = nullptr);

		const Model& get_model(cstr name);

		void translate(vec3 translate, int id);
		void rotate(vec3 axis, float angle, int id);
		void scale(vec3 scale, int id);

		// Getters
		const MapModel&	models()	{ return c_models; }
		Shader*		shader()	{ return c_shader; }
		Material*	material()	{ return c_material; }

		void init();

		void update();

		void render();


	private:
		bool has_own_shader;

		Shader*		c_shader;
		Material*	c_material;

		MapModel c_models;
	};
}