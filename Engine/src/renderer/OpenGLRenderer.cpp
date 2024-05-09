#include <stdexcept>
#include <glew/glew.h>
#include <renderer/OpenGLRenderer.hpp>
#include <core/EngineEvents.hpp>

void ResizeCallback(WindowResizeEvent* e) {
	glViewport(0, 0, e->width, e->height);
}

OpenGLRenderer::OpenGLRenderer() {
try {
	this->InitGlew();
	glm::vec2 wnd_data = Window::GetRect();
	glViewport(0, 0, wnd_data.x, wnd_data.y);
	this->MakeCtxCurrent();
	Event::Register<WindowResizeEvent>((EventCallback)ResizeCallback);
}
catch (std::runtime_error e) {
	throw e;
}}

void OpenGLRenderer::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	this->SwapBuffers();
}

OpenGLRenderer::~OpenGLRenderer() {

}

