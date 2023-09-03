#include "twob_renderer.h"
#include "twob_graphics.h"
#include "twob_canvas.h"
#include "twob_time.h"
#include "twob_filesystem.h"
#include "twob_camera.hpp"
#include "twob_aliases.h"
#include <glad/glad.h>
#include <iostream>
#include "internal/twob_internal.h"

namespace
{
	using namespace twob;
	cstr default_vertex_source = R"(
		#version 330 core
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 normals;
		layout (location = 2) in vec2 texture_coords;

		out vec2 _texture_coords;

		uniform mat4 projection;
		uniform mat4 view;
		uniform mat4 model;

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0);
			_texture_coords = texture_coords;
		} 
	)";
	cstr default_fragment_source = R"(
		#version 330 core
		out vec4 FragColor;

		in vec2 _texture_coords;

		uniform sampler2D texture_data;

		void main()
		{
			FragColor = texture(texture_data, _texture_coords);
		}
	)";

	Mesh cube_mesh(cstr texture_path)
	{
		Mesh cube;
		vector<vertex> vertices;
		vertices.insert(vertices.end(),
			{
				//  vertices					normals					 texture coords
				{   vec3(-0.5f, -0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f) },
				{	vec3(0.5f, -0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 0.0f) },
				{	vec3(0.5f, 0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f) },
				{	vec3(0.5f, 0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(1.0f, 1.0f) },
				{	vec3(-0.5f, 0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 1.0f) },
				{	vec3(-0.5f, -0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f) },

				{   vec3(-0.5f, -0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f) },
				{	vec3(0.5f, -0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f) },
				{	vec3(-0.5f, 0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f) },
				{	vec3(-0.5f, -0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f),	vec2(0.0f, 0.0f) },

				{   vec3(-0.5f, 0.5f, 0.5f),	vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f) },
				{	vec3(-0.5f, 0.5f, -0.5f),	vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f) },
				{	vec3(-0.5f, -0.5f, -0.5f),	vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) },
				{	vec3(-0.5f, -0.5f, -0.5f),	vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) },
				{	vec3(-0.5f, -0.5f, 0.5f),	vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f) },
				{	vec3(-0.5f, 0.5f, 0.5f),	vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f) },

				{   vec3(0.5f, 0.5f, 0.5f),		vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f) },
				{	vec3(0.5f, 0.5f, -0.5f),	vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f) },
				{	vec3(0.5f, -0.5f, -0.5f),	vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) },
				{	vec3(0.5f, -0.5f, -0.5f),	vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f) },
				{	vec3(0.5f, -0.5f, 0.5f),	vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(1.0f, 0.0f, 0.0f),	vec2(1.0f, 0.0f) },

				{   vec3(-0.5f, -0.5f, -0.5f),	vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 1.0f) },
				{	vec3(0.5f, -0.5f, -0.5f),	vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 1.0f) },
				{	vec3(0.5f, -0.5f, 0.5f),	vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f) },
				{	vec3(0.5f, -0.5f, 0.5f),	vec3(0.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f) },
				{	vec3(-0.5f, -0.5f, 0.5f),	vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f) },
				{	vec3(-0.5f, -0.5f, -0.5f),	vec3(0.0f, -1.0f, 0.0f), vec2(0.0f, 1.0f) },

				{   vec3(-0.5f, 0.5f, -0.5f),	vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f) },
				{	vec3(0.5f, 0.5f, -0.5f),	vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f) },
				{	vec3(-0.5f, 0.5f, 0.5f),	vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f) },
				{	vec3(-0.5f, 0.5f, -0.5f),	vec3(0.0f, 1.0f, 0.0f),	vec2(0.0f, 1.0f) }
			}
		);

		cube.vertices = vertices;
		cube.texture = Internal::app_renderer()->create_texture(texture_path);
		cube.color = Color::purple();
		return cube;
	}
}

namespace twob
{

	class Material_OpenGL : public Material
	{
	private:
		int some_material;

	public:
		Material_OpenGL()
		{
			some_material = 1;
		}
	};

	class Shader_OpenGL : public Shader
	{
	private:
		u_int program_ref;

	public:
		Shader_OpenGL()
		{
			create();
		}

		Shader_OpenGL(cstr vertex_source, cstr fragment_source)
		{
			create(vertex_source, fragment_source);
		}

		void create() override
		{
			this->create(default_vertex_source, default_fragment_source);
		}

		void create(cstr vertex_source, cstr fragment_source) override
		{
			GLuint vertex_ref, fragment_ref;
			vertex_ref = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_ref, 1, &vertex_source, NULL);
			glCompileShader(vertex_ref);
			check_compile_errors(vertex_ref, "VERTEX");

			fragment_ref = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_ref, 1, &fragment_source, NULL);
			glCompileShader(fragment_ref);
			check_compile_errors(fragment_ref, "FRAGMENT");

			this->program_ref = glCreateProgram();
			glAttachShader(this->program_ref, vertex_ref);
			glAttachShader(this->program_ref, fragment_ref);

			glLinkProgram(this->program_ref);
			check_compile_errors(this->program_ref, "PROGRAM");

			glDeleteShader(vertex_ref);
			glDeleteShader(fragment_ref);
		}

		void use() override
		{
			glUseProgram(this->program_ref);
		}

		void set_projection() override
		{
			set_value("projection", perspective(radians(45.0f), (float)App::config()->width / (float)App::config()->height, 0.1f, 1000.0f));
		}

		void set_value(cstr name, bool& value) override
		{
			glUniform1i(glGetUniformLocation(this->program_ref, name), value);
		}

		void set_value(cstr name, float& value) override
		{
			glUniform1f(glGetUniformLocation(this->program_ref, name), value);
		}

		void set_value(cstr name, int& value) override
		{
			glUniform1i(glGetUniformLocation(this->program_ref, name), value);
		}

		void set_value(cstr name, const vec2& value) override
		{
			glUniform2f(glGetUniformLocation(this->program_ref, name), value.x, value.y);
		}

		void set_value(cstr name, const vec3& value) override
		{
			glUniform3f(glGetUniformLocation(this->program_ref, name), value.x, value.y, value.z);
		}

		void set_value(cstr name, const vec4& value) override
		{
			glUniform4f(glGetUniformLocation(this->program_ref, name), value.x, value.y, value.z, value.w);
		}

		void set_value(cstr name, const mat4& value) override
		{
			glUniformMatrix4fv(glGetUniformLocation(this->program_ref, name), 1, false, value_ptr(value));
		}

		void check_compile_errors(u_int shader_ref, cstr type)
		{
			int success;
			char infoLog[1024];
			if (type != "PROGRAM")
			{
				glGetShaderiv(shader_ref, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader_ref, 1024, NULL, infoLog);
					cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
						<< infoLog << "\n -- -------------------- -- " << endl;
				}
			}
			else
			{
				glGetProgramiv(shader_ref, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader_ref, 1024, NULL, infoLog);
					cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
						<< infoLog << "\n -- ---------------------- -- " << endl;
				}
			}
		}
	};

	class Texture_OpenGL : public Texture
	{
	public:
		Texture_OpenGL(cstr file_path)
		{
			this->create(file_path);
		}

		void create(cstr file_path) override
		{
			image = new Image(file_path);

			glGenTextures(1, &texture_ref);

			glBindTexture(GL_TEXTURE_2D, texture_ref);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->image_data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	};

	class FrameBuffer_OpenGL : public FrameBuffer
	{

	};

	class Model_OpenGL : public Model
	{
	private:
		GLuint vertex_array_object;

	public:
		Model_OpenGL(Polygon shape, cstr texture_path)
		{
			switch (shape)
			{
			case twob::CUBE:
				meshes.push_back(cube_mesh(texture_path));
				break;
			case twob::SPHERE:
				break;
			case twob::PYRAMID:
				break;
			}
			model_matrix = mat4(1.0f);

			model_data_to_gpu();
		}

		Model_OpenGL(cstr path) 
		{
			meshes = File::load_meshes_from_file(path);
			model_matrix = mat4(1.0f);
		}

		void translate(vec3 translate) override
		{
			model_matrix = glm::translate(model_matrix, translate);
		}

		void scale(vec3 scale) override
		{
			model_matrix = glm::scale(model_matrix, scale);
		}

		void rotate(vec3 axis, float angle) override
		{
			model_matrix = glm::rotate(model_matrix, radians(angle), axis);
		}

		void render(Shader *shader) override
		{
			shader->set_value("model", model_matrix);

			glBindTexture(GL_TEXTURE_2D, meshes[0].texture->texture_ref);

			glBindVertexArray(vertex_array_object);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		mat4 get_model_matrix() 
		{
			return model_matrix;
		}

	private:
		void model_data_to_gpu()
		{
			glGenVertexArrays(1, &vertex_array_object);

			GLuint vertex_buffer_object;
			glGenBuffers(1, &vertex_buffer_object);

			glBindVertexArray(vertex_array_object);

			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(vec3) + sizeof(vec3) + sizeof(vec2)) * meshes[0].vertices.size(), meshes[0].vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

			glBindVertexArray(0);
		}
	};

	class Renderer_OpenGL : public Renderer
	{
	private:
		Camera* active_camera;
		Viewport* viewport = new Viewport(0, 0, 0, 0);

	public:
		void init() override
		{
			glEnable(GL_DEPTH_TEST);
			active_camera = create_camera();

			set_viewport();
		}

		void render(Cluster& cluster) override
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cluster.shader()->use();
			cluster.shader()->set_value("view", active_camera->view_matrix());
			
			MapModel::const_iterator it = cluster.models().begin();

			while( it != cluster.models().end())
			{
				Model& model = *(it->second);
				model.render(cluster.shader());
				it++;
			}

		}

		void set_viewport() override
		{
			*viewport = { 0, 0, App::config()->width, App::config()->height };
			glViewport(viewport->x, viewport->y, viewport->w, viewport->h);
		}

		// Factory Methods //

		Shader* create_shader(cstr vertex_source = nullptr, cstr fragment_source = nullptr) override
		{
			if (vertex_source == nullptr || fragment_source == nullptr)
				return new Shader_OpenGL();
			else
				return new Shader_OpenGL(vertex_source, fragment_source);
		}

		Material* create_material(MaterialData* material = nullptr) override
		{
			if (material == nullptr)
				return new Material_OpenGL(); // return default material
			else
				return nullptr;// Create OpenGL material based on properties
		}

		Model* create_model(cstr file_path) override
		{
			return nullptr;
		}

		Model* create_model(Polygon shape, cstr texture_path) override
		{
			return new Model_OpenGL(shape, texture_path);
		}

		Texture* create_texture(cstr texture_path) override
		{
			return new Texture_OpenGL(texture_path);
		}

		Camera* create_camera() override
		{
			return new Camera();
		}

		Camera* camera() override
		{
			return active_camera;
		}
	};

}

twob::Renderer* twob::Renderer::instance_opengl()
{
	return new twob::Renderer_OpenGL();
}
