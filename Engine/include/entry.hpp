#pragma once

#include <core/Application.hpp>

extern IApplication* GetConfiguredApplication();

int main() {
	IApplication* app = GetConfiguredApplication();
	Engine e;
	e.Initialize(app);
	int ret = e.Run();
	e.Shutdown();
	delete app;
	return ret;
}