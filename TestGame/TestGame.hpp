#pragma once

#include <core/Application.hpp>

class TestGame : public IApplication {
public:
	TestGame();
	bool OnInit() override;
	bool OnUpdate(float delta_time) override;
	bool OnRender() override;
	bool OnShutdown() override;
};