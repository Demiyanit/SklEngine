#include <iostream>
#include <stdexcept>
#include <core/Application.hpp>
#include <core/EngineEvents.hpp>
#include <core/Engine.hpp>
#include <platform/Window.hpp>
#include <core/Input.hpp>
#include <renderer/OpenGLRenderer.hpp>

IRenderer* Engine::renderer = nullptr;
IApplication* Engine::application_instance = nullptr;

void InitShutdownSequence(EngineCloseEvent* e) {
	Window::SetShouldClose(true);
	if (e->error_code < 0) {
		std::cerr << "Error: " << e->message << std::endl;
		return;
	}
	std::cout << e->message << std::endl;
}

void WindowResizeHandler(WindowResizeEvent* e) {
	
	std::cout << "Window resized to: " << e->width << ", " << e->height << std::endl;
}

void Engine::Initialize(IApplication* inst) {
	Engine::application_instance = inst;
	
	try {
		Window::Init(inst->ApplicationName.c_str(), inst->width, inst->height);
		Input::Init();
		Engine::renderer = new OpenGLRenderer();
		Event::Register<EngineCloseEvent>((EventCallback)InitShutdownSequence);
		Event::Register<WindowResizeEvent>((EventCallback)WindowResizeHandler);
	}
	catch (std::exception e) {
		std::cerr << "[ERROR]: Engine failed to initialize... \n" << e.what() << "\nExiting..." << std::endl;
		exit(-1);
	}
}

int Engine::Run() {
try {
	Engine::application_instance->OnInit();
	while(!Window::ShouldClose()) {
		Window::Update();
		Input::Update();
		application_instance->OnRender();
		renderer->Render();
	}
	Engine::application_instance->OnShutdown();
	
}
catch (std::exception e) {
	std::cerr << "[ERROR]: Engine failed to update...\n" << e.what() << "\n Shutting down..." << std::endl;
	Engine::application_instance->OnShutdown();
	return -1;
}return 0;
}

void Engine::Shutdown() {
	Window::Shutdown();
}
