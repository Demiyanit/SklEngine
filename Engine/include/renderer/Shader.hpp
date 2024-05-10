#pragma once

class Renderer;

class Shader {
public:
	void* data;
private:
	friend class Renderer;
};