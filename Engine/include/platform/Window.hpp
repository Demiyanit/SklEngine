#pragma once

#include <glm/glm.hpp>
#include <string>

class Window {
public:
	static Window*& Get() {
		return instance;
	}

	static void Init(const char* name, int width, int height);
	static void Shutdown();
	void SetTitle(const char* title);
	std::string GetTitle();
	void Update();
	glm::vec2 GetRect();

	void* wnd_handle = nullptr;
private:
	Window(const char* name, int width, int height);
	~Window();

	static Window* instance;
};