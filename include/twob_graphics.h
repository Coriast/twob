#pragma once
#include <glad/glad.h>
#include <twob_color.h>
#include <twob_aliases.h>
#include <twob_image.h>
#include <vector>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

using namespace glm;

namespace twob
{
	class Texture;

	enum RendererType
	{
		OpenGL,
		Vulkan
	};

	enum Polygon
	{
		CUBE,
		SPHERE,
		PYRAMID
	};

	enum ModelType
	{
		wavefront,
		gltf
	};

	struct Viewport
	{
		int x, y, w, h;
		Viewport(int x, int y, int w, int h) 
			: x(x), y(y), w(w), h(h) {};
	};

	struct vertex
	{
		vec3 position;
		vec3 normal;
		vec2 tex_coord;
	};

	struct MaterialData
	{

	};

	class Material
	{

	};

	class Shader
	{
	protected:
		Material* material;

	public:

		virtual void create() = 0;
		virtual void create(cstr vertex_source, cstr fragment_source) = 0;

		virtual void use() = 0;

		virtual void set_projection() = 0;

		virtual void set_value(cstr name, bool value) = 0;
		virtual void set_value(cstr name, int& value) = 0;
		virtual void set_value(cstr name, float& value) = 0;
		virtual void set_value(cstr name, const vec2& value) = 0;
		virtual void set_value(cstr name, const vec3& value) = 0;
		virtual void set_value(cstr name, const vec4& value) = 0;
		virtual void set_value(cstr name, const mat4& value) = 0;
	};

	class FrameBuffer
	{

	};

	class Mesh
	{
	public:
		bool has_texture = false;
		std::vector<vertex>	vertices;
		std::vector<u_int>	indices;
		Texture* texture;
		Color color;

		virtual void set_tex(cstr texture_path) = 0;
		virtual void set_color(Color color) = 0;
		virtual void generate_buffers() = 0;
		virtual void set_gpu_data() = 0;

	};

	class Model
	{
	protected:
		mat4 model_matrix;
	
	public:
		std::vector<Mesh*> meshes;

		virtual void translate(vec3 translate) = 0;
		virtual void scale(vec3 scale) = 0;
		virtual void rotate(vec3 axis, float angle) = 0;

		virtual void render(Shader* shader) = 0;
	};

	class Texture
	{
	public:
		Image* image;
		GLuint texture_ref;

		virtual void create(cstr file_path) = 0;
	};
}