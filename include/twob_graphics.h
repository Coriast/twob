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
	public:
		virtual void create() = 0;
		virtual void create(const char* vertex_source, const char* fragment_source) = 0;

		virtual void use() = 0;

		virtual void set_value(const char* name, bool& value) = 0;
		virtual void set_value(const char* name, float& value) = 0;
		virtual void set_value(const char* name, int& value) = 0;
		virtual void set_value(const char* name, const vec2& value) = 0;
		virtual void set_value(const char* name, const vec3& value) = 0;
		virtual void set_value(const char* name, const vec4& value) = 0;
		virtual void set_value(const char* name, const mat4& value) = 0;

		virtual void check_compile_errors(u_int shader_ref, const char* type) = 0;
	};

	class Texture
	{
	public:
		virtual void create(const char* file_path) = 0;
	};

	class Camera
	{
	public:
		virtual mat4 get_view_matrix() = 0;

		virtual void process_keyboard(Direction direct) = 0;
		virtual void process_mouse(float x_offset, float y_offset) = 0;
		virtual void update_vectors() = 0;
	};

	class FrameBuffer
	{

	};

	class Model
	{
	public:
		virtual void transform(vec3 transform) = 0;
		virtual void scale(vec3 scale) = 0;
		virtual void rotate(vec3 axis, float angle) = 0;

	};

	class DrawCall
	{

	private:
		
	};
}