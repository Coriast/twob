#pragma once 
#include <string>
#include <twob_graphics.h>

namespace twob
{
	class Path
	{
	public:
		static std::string get_absolute_path();
		static std::string get_absolute_path(const char* filename);
	};

	class File
	{
	public:
		static vector<Mesh> load_meshes_from_file(const char* path) 
		{ 
			vector<Mesh> file_meshes;
			return file_meshes; 
		};
	};
}