#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <core/Application.hpp>
#include <core/EngineEvents.hpp>
#include <core/Engine.hpp>
#include <platform/Window.hpp>
#include <platform/Timer.hpp>
#include <core/Input.hpp>
#include <renderer/Renderer.hpp>
#include <util/Load.hpp>

IApplication* Engine::application_instance = nullptr;
Engine* Engine::instance = nullptr;

void InitShutdownSequence(EngineCloseEvent* e) {
	Window::SetShouldClose(true);
	if (e->error_code < 0) {
		std::cerr << "Error: " << e->message << std::endl;
		return;
	}
	std::cout << e->message << std::endl;
}

void Engine::Initialize(IApplication* inst) {
	Engine::application_instance = inst;
	Timer::Initialize();
	try {
		Window::Init(inst->ApplicationName.c_str(), inst->width, inst->height);
		Input::Init();
		this->renderer = RendererGet("VulkanRenderer");
		Event::Register<EngineCloseEvent>((EventCallback)InitShutdownSequence);
	}
	catch (std::exception e) {
		std::cerr << "[ERROR]: Engine failed to initialize... \n" << e.what() << "\nExiting..." << std::endl;
		exit(-1);
	}
}

int Engine::Run() {
try {
	while (!Window::ShouldClose()) {
		Input::Update();
		Window::Update();
	}
} catch (std::exception e) {
	std::cerr << "[ERROR]: Engine failed to update...\n" << e.what() << "\n Shutting down..." << std::endl;
	Engine::application_instance->OnShutdown();
	return -1;
}
return 0;
}

void Engine::Shutdown() {
	Window::Shutdown();
	if (renderer && renderer->Shutdown) {
		renderer->Shutdown();
	}
	delete renderer;
}

Engine* GetEngineInstance() {
	if (!Engine::instance) { Engine::instance = new Engine(); }
	return Engine::instance;
}
