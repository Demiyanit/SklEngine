#include <iostream>

#include <core/Application.hpp>
#include <core/EngineEvents.hpp>
#include <core/Engine.hpp>
#include <platform/Window.hpp>
#include <core/Input.hpp>
Engine::Engine(IApplication* inst) {
	this->application_instance = inst;
	inst->engine = this;
	Window::Init(inst->ApplicationName.c_str(), inst->width, inst->height);
	Input::Init();
}

int Engine::run() {
	if (!this->application_instance->OnInit()) {
		std::cerr << "Failed to initialize application. Shutting down..." << std::endl;
		return -1;
	}
	while(true) {
		Window::Get()->Update();
		Input::Update();
		if (!application_instance->OnRender()) {
			std::cerr << "OnRender() failed, Shutting down..." << std::endl;
		}
	}
	if (!this->application_instance->OnShutdown()) {
		std::cerr << "Failed to shutdown application correctly. Shutting down..." << std::endl;
		return -2;
	}
}

Engine::~Engine() {
	Window::Shutdown();
}