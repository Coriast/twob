#pragma once
#include <twob_graphics.h>
#include <unordered_map>
#include <glm/vec3.hpp>

using namespace glm;
using namespace std;

namespace twob
{
	typedef int ModelId;
	typedef unordered_map<ModelId, Model*> HashModel;

	/* 
	* Some kind of All-in- one class. The Cluster's principle is to:
	* -> Be a 3D models basket, where you can push all the models that
	*    will use the same Shader + Material configurations.
	* 
	* -> That way you can call just one render for each bunch of models 
	*	 that'll use the same configurations.
	* 
	* -> Probably camera will be set apart of the rendering specific architecture
	*/
	class Cluster
	{
	public:
		Cluster();

		// load content 
		void load_shader(cstr vertex_source, cstr fragment_source);
		void load_material();

		void load_model(cstr file_path, ModelId m);
		void load_model(Polygon shape, ModelId m);

		void translate(vec3 translate, ModelId m);
		void rotate(vec3 axis, float angle, ModelId m);
		void scale(vec3 scale, ModelId m);

		// Getters
		HashModel	models()	{ return c_models; }
		Shader*		shader()	{ return c_shader; }
		Material*	material()	{ return c_material; }
		Camera*		camera()	{ return c_camera; }
		Viewport*	viewport()	{ return c_viewport; }

		void init();

		void render();


	private:
		bool has_own_shader;

		Viewport*	c_viewport;
		Shader*		c_shader;
		Material*	c_material;
		Camera*		c_camera;

		HashModel c_models;
	};
}