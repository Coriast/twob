#include <twob_image.h>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

using namespace twob;

Image::Image(std::string file_path)
{
	stbi_set_flip_vertically_on_load(true);
	image_data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
}

Image::~Image()
{
	stbi_image_free(image_data);
}
