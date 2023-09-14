#pragma once 
#include <string>
#include <twob_graphics.h>

namespace twob
{
	class Path
	{
	public:
		static std::string get_absolute_path();
		static std::string get_absolute_path(cstr filename);
	};

	class File
	{
	public:
		static void load_meshes_from_obj(cstr folder_path, std::vector<Mesh*> &meshes);

	};
}