#pragma once
#include <twob_aliases.h>
#include <twob_color.h>
#include <twob_image.h>
#include <vector>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

namespace twob
{
	class Texture;

	enum RendererType
	{
		OpenGL,
		Vulkan
	};

	enum Direction
	{
		FORWARD,
		BACKWARD,
		RIGHT,
		LEFT
	};

	enum Polygon
	{
		CUBE,
		SPHERE,
		PYRAMID
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

	struct Mesh
	{
		vector<vertex>	vertices;
		vector<u_int>	indices;
		vector<Texture> textures;

		Color color;
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

		virtual void set_value(cstr name, bool& value) = 0;
		virtual void set_value(cstr name, float& value) = 0;
		virtual void set_value(cstr name, int& value) = 0;
		virtual void set_value(cstr name, const vec2& value) = 0;
		virtual void set_value(cstr name, const vec3& value) = 0;
		virtual void set_value(cstr name, const vec4& value) = 0;
		virtual void set_value(cstr name, const mat4& value) = 0;
	};

	class Texture
	{
	public:
		virtual void create(cstr file_path) = 0;
	};

	class Camera
	{
	public:

		virtual mat4 view_matrix() = 0;
		virtual void process_keyboard(Direction direct) = 0;
		virtual void process_mouse(float x_offset, float y_offset) = 0;
		virtual void update_vectors() = 0;
	};

	class FrameBuffer
	{

	};

	class Model
	{
	protected:
		vector<Mesh> meshes;
		mat4		 model_matrix;
	
	public:
		virtual void translate(vec3 translate) = 0;
		virtual void scale(vec3 scale) = 0;
		virtual void rotate(vec3 axis, float angle) = 0;

		virtual void render(Shader* shader) = 0;
	};
}