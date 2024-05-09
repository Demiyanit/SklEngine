#pragma once

#include <util/Event.hpp>
#include <string>

class EngineCloseEvent : public Event {
public:
	//If error code >= 0; exit successfully
	//If < 0; Error out
	EngineCloseEvent(int error_code = 0, std::string message = "") : error_code(error_code), message(message) {}
	int error_code;
	std::string message;
};

class InputKeyEvent : public Event {
public:
	InputKeyEvent(int key, bool down) : key(key), down(down) {}
	int key = 0;
	bool down;
};
class InputMouseButtonEvent : public Event {
public:
	InputMouseButtonEvent(int btn, bool down) : btn(btn), down(down) {}
	int btn = 0;
	bool down;
};
class InputMouseMoveEvent : public Event {
public:
	InputMouseMoveEvent(int x, int y) : x(x), y(y) {}
	int x = 0;
	int y = 0;
};

class WindowResizeEvent : public Event {
public:
	WindowResizeEvent(int width, int height) : width(width), height(height) {}
	int width;
	int height;
};