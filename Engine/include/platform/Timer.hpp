#pragma once

#include <core/EngineEvents.hpp>
#include <cstdint>

class Timer {
	static double frequency;
	friend class Engine;
	static void Initialize();
	static double GetAbsoluteTime();
	static void Sleep(unsigned long duration);
};