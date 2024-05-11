#pragma once

#include<vector>

class Renderer;

class Mesh {
public:
	unsigned int verticies_count;
	unsigned int indices_count;
	float* verticies;
	unsigned int* indices;
	void* data;
private:
	friend class Renderer;
};