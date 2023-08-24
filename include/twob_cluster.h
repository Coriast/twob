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

	class Cluster
	{
		friend class Renderer;
	public:
		Cluster();

		void insert_model(const char* file_path, ModelId m);
		void insert_model(Polygon shape, ModelId m);

		void transform(vec3 transform, ModelId m);
		void rotate(vec3 axis, float angle, ModelId m);
		void scale(vec3 scale, ModelId m);

		void render();

	private:
		bool has_viewport;
		bool has_own_shader;

		Viewport	viewport;
		Shader*		shader;
		Model*		model;
		Material*	material;

		HashModel models;
	};
}