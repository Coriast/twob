#pragma once
#include <twob_graphics.h>
#include <twob_cluster.h>

namespace twob
{
	class Renderer
	{
	public:

		virtual void init() = 0;

		virtual void render(Cluster& cluster) = 0;

		virtual Material* create_material(MaterialData* material = nullptr) = 0;
		virtual Model* create_model(const char* file_path) = 0;
		virtual Model* create_model(Polygon shape) = 0;

	private:
		static Renderer* instance_opengl();
		static Renderer* instance_vulkan() { return nullptr; };

	protected:
		static Viewport& viewport(Cluster& cluster) { return cluster.viewport; } ;

	public:
		static Renderer* instance_renderer(RendererType type)
		{
			switch (type)
			{
			case OpenGL:
				return instance_opengl();
				break;
			case Vulkan:
				return instance_vulkan();
				break;
			default:
				break;
			}
		}
	};
}