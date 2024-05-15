#pragma once

#include <vector>
#include <string>

class Renderer;

class Shader {
public:
	int uid;
	std::vector<std::string> paths;
	void* data;
private:
	friend class Renderer;
};