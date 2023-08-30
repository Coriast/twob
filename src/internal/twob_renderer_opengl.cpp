#include "twob_renderer.h"
#include "twob_graphics.h"
#include "twob_canvas.h"
#include "twob_time.h"
#include "twob_filesystem.h"
#include "twob_aliases.h"
#include <glad/glad.h>
#include <iostream>

namespace
{
	using namespace twob;
	cstr default_vertex_source = R"(
		#version 330 core
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec2 texture_coords;

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
			FragColor = vec4(0.3f, 0.7f, 0.2f, 1.0f);//texture(texture_data, _texture_coords);
		}
	)";

	Mesh cube_mesh()
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
				{	vec3(-0.5f, 0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f) },
				{	vec3(-0.5f, -0.5f, -0.5f),	vec3(0.0f, 0.0f, -1.0f), vec2(0.0f, 0.0f) },

				{   vec3(-0.5f, -0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f) },
				{	vec3(0.5f, -0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f) },
				{	vec3(0.5f, 0.5f, 0.5f),		vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f) },
				{	vec3(-0.5f, 0.5f, 0.5f),	vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f) },
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
	private:
		Image* image;
		GLuint texture_ref;

	public:
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

	class Camera_OpenGL : public Camera
	{
	private:
		vec3 position	= vec3(0.0f, 0.0f, 3.0f);
		vec3 front		= vec3(0.0f, 0.0f, -1.0f);
		vec3 up			= vec3(0.0f, 1.0f, 0.0f);
		vec3 right;
		vec3 world_up;

		float yaw, pitch;

		float movement_speed;
		float mouse_sensitivity;

	public:
		mat4 view_matrix() override
		{
			return lookAt(position, position + front, up);
		}

		void process_keyboard(Direction direct) override
		{
			float velocity = movement_speed * Time::delta();
			if (direct == Direction::FORWARD)
			{
				position += front * velocity;
			}
			if (direct == Direction::BACKWARD)
			{
				position -= front * velocity;
			}
			if (direct == Direction::LEFT)
			{
				position -= right * velocity;
			}
			if (direct == Direction::RIGHT)
			{
				position += right * velocity;
			}
		}

		void process_mouse(float x_offset, float y_offset) override
		{
			x_offset *= mouse_sensitivity;
			y_offset *= mouse_sensitivity;

			yaw += x_offset;
			pitch += y_offset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			update_vectors();
		}

		void update_vectors() override
		{
			vec3 front;
			front.x = cos(radians(yaw)) * cos(radians(pitch));
			front.y = sin(radians(pitch));
			front.z = sin(radians(yaw)) * cos(radians(pitch));
			front = normalize(front);

			right = normalize(cross(front, world_up));
			up = normalize(cross(right, front));
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
		Model_OpenGL(Polygon shape)
		{
			switch (shape)
			{
			case twob::CUBE:
				meshes.push_back(cube_mesh());
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

			glBindVertexArray(0);
		}
	};

	class Renderer_OpenGL : public Renderer
	{
	public:
		void init() override
		{
			glEnable(GL_DEPTH_TEST);
		}

		void render(Cluster& cluster) override
		{

			Viewport* vp = viewport(cluster);
			glViewport(vp->x, vp->y, vp->w, vp->h);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cluster.shader()->use();
			cluster.shader()->set_value("projection", perspective(radians(45.0f), (float)vp->w / (float)vp->h, 0.1f, 1000.0f));
			cluster.shader()->set_value("view", cluster.camera()->view_matrix());

			cluster.models().at(0)->render(cluster.shader());

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

		Model* create_model(Polygon shape) override
		{
			return new Model_OpenGL(shape);
		}

		Camera* create_camera() override
		{
			return new Camera_OpenGL();
		}
	};

}

twob::Renderer* twob::Renderer::instance_opengl()
{
	return new twob::Renderer_OpenGL();
}
