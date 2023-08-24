#pragma once
#include <string>

namespace twob
{
	class Image
	{
	public:
		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* image_data = nullptr;

		Image(std::string file_path);
		~Image();
	};

}
