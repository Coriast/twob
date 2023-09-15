#include "twob_renderer.h"
#include "twob_graphics.h"
#include "twob_canvas.h"
#include "twob_time.h"
#include "twob_filesystem.h"
#include "twob_camera.h"
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
		//layout (location = 3) in vec3 _color_; // probably send color only one time

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

		uniform vec3 color;
		uniform bool has_texture;
		uniform sampler2D texture_data;

		void main()
		{
			if(has_texture)
			{
				FragColor = texture(texture_data, _texture_coords);
			}
			else 
			{
				FragColor = vec4(color, 1.0f);
			}
		}
	)";
	
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

		void set_value(cstr name, bool value) override
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
					std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
						<< infoLog << "\n -- -------------------- -- " << std::endl;
				}
			}
			else
			{
				glGetProgramiv(shader_ref, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(shader_ref, 1024, NULL, infoLog);
					std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
						<< infoLog << "\n -- ---------------------- -- " << std::endl;
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->image_data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

	class FrameBuffer_OpenGL : public FrameBuffer
	{

	};

	class Mesh_OpenGL : public Mesh
	{
	public:

		GLuint vertex_buffer_object;
		GLuint element_buffer_object;

		void set_tex(cstr texture_path) override
		{
			Texture* tex = Internal::app_renderer()->create_texture(texture_path);
			this->texture = tex;
			this->has_texture = true;
		}

		void set_color(Color color) override
		{
			this->color = color;
		}

		void generate_buffers() override
		{
			glGenBuffers(1, &vertex_buffer_object);
			glGenBuffers(1, &element_buffer_object);
		}

		void set_gpu_data() override
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(vec3) + sizeof(vec3) + sizeof(vec2)) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			if (!indices.empty())
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
			}

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
	};

	class Model_OpenGL : public Model
	{
	private:
		GLuint vertex_array_object;

	public:
		Model_OpenGL(Polygon shape)
		{
			switch (shape)
			{
			case twob::CUBE:
			{
				Mesh_OpenGL* cube = new Mesh_OpenGL();
				std::vector<vertex> vertices;
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

				cube->vertices.insert(cube->vertices.end(), vertices.begin(), vertices.end());
				meshes.push_back(cube);
			}
				break;
			case twob::SPHERE:
				break;
			case twob::PYRAMID:
				break;
			}
			model_matrix = mat4(1.0f);

			model_data_to_gpu();
		}

		Model_OpenGL(ModelType type, cstr folder_path) 
		{
			if (type == wavefront)
			{
				File::load_meshes_from_obj(folder_path, meshes);

			}
			model_matrix = mat4(1.0f);

			model_data_to_gpu();
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

			for (int i = 0; i < meshes.size(); i++)
			{
				shader->set_value("has_texture", meshes[i]->has_texture);
				if (meshes[i]->has_texture)
				{
					glBindTexture(GL_TEXTURE_2D, meshes[i]->texture->texture_ref);
				}
				else
				{
					vec3 s_color(meshes[i]->color.r, meshes[i]->color.g, meshes[i]->color.b);
					shader->set_value("color", s_color);
				}
			}

			// while i don't have any model with multiple meshes i'm gonna draw only one 
			glBindVertexArray(vertex_array_object);
			if (!meshes[0]->indices.empty())
				glDrawElements(GL_TRIANGLES, meshes[0]->indices.size(), GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, meshes[0]->vertices.size());
			glBindVertexArray(0);
		}

		mat4 get_model_matrix() 
		{
			return model_matrix;
		}

	private:
		void model_data_to_gpu()
		{
			glGenVertexArrays(1, &vertex_array_object);

			for (int i = 0; i < meshes.size(); i++)
				meshes[i]->generate_buffers();

			glBindVertexArray(vertex_array_object);

			for (int i = 0; i < meshes.size(); i++)
				meshes[i]->set_gpu_data();

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

		Mesh* create_mesh(std::vector<vertex> vertices) override
		{
			Mesh_OpenGL* mesh = new Mesh_OpenGL();
			mesh->vertices = vertices;
			return mesh;
		}

		Model* create_model(ModelType type, cstr folder_path) override
		{
			Model_OpenGL* model = new Model_OpenGL(type, folder_path);
			return model;
		}

		Model* create_primitive(Polygon shape) override
		{
			return new Model_OpenGL(shape);
		}

		Texture* create_texture(cstr texture_path) override
		{
			if (texture_path != nullptr)
				return new Texture_OpenGL(texture_path);
			else
				return nullptr;
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
