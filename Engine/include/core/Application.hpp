#pragma once

#include <core/Engine.hpp>
#include <string>

class SKLC_API IApplication {
private:
	friend class Engine;
public:
	virtual void OnInit() = 0;
	virtual void OnUpdate(float delta_time) = 0;
	virtual void OnRender() = 0;
	virtual void OnShutdown() = 0;
	virtual ~IApplication() = default;
/**
* Application config data
*/

	std::string ApplicationName = "";
	int width = -1;
	int height= -1;

	Engine* engine;
};