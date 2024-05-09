#pragma once

#include <core/Engine.hpp>
#include <string>

class SKLC_API IApplication {
private:
	friend class Engine;
	Engine* engine = nullptr;
public:
	virtual bool OnInit() = 0;
	virtual bool OnUpdate(float delta_time) = 0;
	virtual bool OnRender() = 0;
	virtual bool OnShutdown() = 0;
	virtual ~IApplication() = default;
/**
* Application config data
*/
protected:
	std::string ApplicationName = "";
	int width = -1;
	int height= -1;
};