#include <core/EngineEvents.hpp>
#include <core/Input.hpp>
#include <iostream>

bool Input::initialized = false;
bool Input::keys[KEYS_MAX_KEYS] = { false };
bool Input::prev_keys[KEYS_MAX_KEYS] = { false };
bool Input::buttons[MOUSE_MAX_BUTTONS] = { false };
bool Input::prev_buttons[MOUSE_MAX_BUTTONS] = { false };
glm::vec2 Input::mouse_pos = glm::vec2(0, 0);
glm::vec2 Input::mouse_delta = glm::vec2(0, 0);

void ProcessInputKeyEvent(InputKeyEvent e) {
	Input::keys[e.key] = e.down;
}

void ProcessInputMouseEvent(InputMouseButtonEvent e) {
	Input::buttons[e.btn] = e.down;
}

void ProcessInputMouseMoveEvent(InputMouseMoveEvent e) {
	glm::vec2 new_mouse_pos = glm::vec2(e.x, e.y);
	Input::mouse_delta = new_mouse_pos - Input::mouse_pos;
	Input::mouse_pos = new_mouse_pos;
}

void Input::Init() {
	if (initialized) return;
	Event::Register<InputKeyEvent>((EventCallback)ProcessInputKeyEvent);
	Event::Register<InputMouseButtonEvent>((EventCallback)ProcessInputMouseEvent);
	Event::Register<InputMouseMoveEvent>((EventCallback)ProcessInputMouseMoveEvent);
	initialized = true;
}

void Input::Update() {
	memcpy(prev_keys, keys, KEYS_MAX_KEYS * sizeof(bool));
	memcpy(prev_buttons, buttons, MOUSE_MAX_BUTTONS * sizeof(bool));
}

bool Input::KeyPressed(Keys key) {
	return keys[key];
}

bool Input::KeyJustPressed(Keys key) {
	return keys[key] && !prev_keys[key];
}

bool Input::KeyReleased(Keys key) {
	return !keys[key];
}

bool Input::KeyJustReleased(Keys key) {
	return !keys[key] && prev_keys[key];
}

bool Input::MouseButtonPressed(MouseButtons btn) {
	return buttons[btn];
}

bool Input::MouseButtonJustPressed(MouseButtons btn) {
	return buttons[btn] && !prev_buttons[btn];
}

bool Input::MouseButtonReleased(MouseButtons btn) {
	return !buttons[btn];
}

bool Input::MouseButtonJustReleased(MouseButtons btn) {
	return !buttons[btn] && prev_buttons[btn];
}
