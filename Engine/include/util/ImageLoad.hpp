#pragma once

#include <string>
#include <vector>

class ImageLoader {
public:
	static std::vector<unsigned char> LoadImage(const std::string& path);
};
