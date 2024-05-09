#pragma once

#include <core/Application.hpp>

extern IApplication* GetConfiguredApplication();

int main() {
	IApplication* app = GetConfiguredApplication();
	Engine*   instance = new Engine(app);
	
	int ret = instance->run();
	
	delete app;
	delete instance;
	return ret;
}