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

#if defined(_WIN32) && defined(_WIN64)
#include <Windows.h>

Library LibraryLoader::Load(const std::string& path) {
	Library	ret;
	ret.handle = LoadLibraryA(path.c_str());
	if (!ret.handle) {
		throw new std::runtime_error("Failed to get the library: " + path);
	}
	ret.name = path;
	return ret;
}

Library::~Library() {
	if (!FreeLibrary((HMODULE)this->handle)) {
		throw new std::runtime_error("Failed to unload library: " + this->name);
	}
}

void* Library::GetFPtr(const std::string& name) {
	return GetProcAddress((HMODULE)this->handle, name.c_str());
}

#endif


