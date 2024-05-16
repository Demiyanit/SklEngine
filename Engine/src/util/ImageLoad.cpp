#include <stdexcept>

#include <util/Load.hpp>
#include <stb_image.h>

std::vector<unsigned char> ImageLoader::LoadImage(const std::string &path) {
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (data == nullptr)
		throw std::runtime_error("Failed to load image: " + path);

	std::vector<unsigned char> image(data, data + width * height * 4);
	stbi_image_free(data);

	return image;
}
