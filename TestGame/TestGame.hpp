#pragma once

#include <core/Application.hpp>

class TestGame : public IApplication {
public:
	TestGame();
	void OnInit() override;
	void OnUpdate(float delta_time) override;
	void OnRender() override;
	void OnShutdown() override;
};