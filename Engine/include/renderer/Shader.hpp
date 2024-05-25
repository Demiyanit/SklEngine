#pragma once

#include <vector>
#include <string>

class Renderer;

class IShader {
public:
	int uid;
private:
	friend class Renderer;
};