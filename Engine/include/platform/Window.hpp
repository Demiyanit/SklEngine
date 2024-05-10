#pragma once

#include <glm/glm.hpp>
#include <string>


class Renderer;
class Window {
public:

	static void Init(const char* name, int width, int height);
	static void Shutdown();
	static void SetTitle(const char* title);
	static std::string GetTitle();
	static void Update();
	static glm::vec2 GetRect();
	static bool ShouldClose() { return shouldclose; }
	static void SetShouldClose(bool should) { shouldclose = should; }
	static void* wnd_handle;
private:
	static bool shouldclose;
	static bool initialized;
	friend class Renderer;
};