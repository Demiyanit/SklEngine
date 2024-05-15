#if defined(_WIN32) && defined(_WIN64)
#include <platform/Window.hpp>
#include <Windows.h>
#include <stdexcept>
#include <core/EngineEvents.hpp>
#include <renderer/Renderer.hpp>

void* Window::wnd_handle = nullptr;
bool Window::shouldclose = false;
bool Window::initialized = false;

#define GET_X_LPARAM(lParam) ((int)(short)LOWORD(lParam))
#define GET_Y_LPARAM(lParam) ((int)(short)HIWORD(lParam))

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void Window::Init(const char* name, int width, int height) {
	if (Window::initialized) return;
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"SklC";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassExW(&wc)) {
		throw new std::runtime_error("Failed to register window class");
		
	}

	DWORD style = WS_OVERLAPPEDWINDOW;
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, style, FALSE);
	std::string sname(name);
	std::wstring wname(sname.begin(), sname.end());

	wnd_handle = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"SklC",
		wname.c_str(),
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);

	if (wnd_handle == NULL) {
		throw new std::runtime_error("Failed to initialize window");
		
	}
	ShowWindow((HWND)wnd_handle, SW_SHOW);
	UpdateWindow((HWND)wnd_handle);
	GetDC((HWND)wnd_handle);
	initialized = true;
}

void Window::Shutdown() {
	DestroyWindow((HWND)Window::wnd_handle);
	Window::wnd_handle = nullptr;
	initialized = false;
	UnregisterClassW(L"SklC", GetModuleHandleW(NULL));
}

void Window::SetTitle(const char* title) {
	SetWindowTextA((HWND)wnd_handle, title);
}

std::string Window::GetTitle() {
	char title[256];
	GetWindowTextA((HWND)wnd_handle, title, 256);
	return std::string(title);
}

void Window::Update() {
	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

glm::vec2 Window::GetRect() {
	RECT rect = { 0, 0, 0, 0 };
	AdjustWindowRect(&rect, GetWindowLong((HWND)wnd_handle, GWL_STYLE), FALSE);
	return glm::vec2(rect.left, rect.top);
}

void Window::LockCursor(bool lock) {
	if (lock) {
		RECT rect;
		GetClientRect((HWND)wnd_handle, &rect);
		POINT p = { rect.left/2.0f, rect.top/2.0f };
		ClientToScreen((HWND)wnd_handle, &p);
		SetCursorPos(p.x, p.y);
		RECT r = { 1, 1, 1, 1 };
		ClipCursor(&r);
		SetCapture((HWND)wnd_handle);
		ShowCursor(false);
	}
	else {
		ReleaseCapture();
		ClipCursor(NULL);
		ShowCursor(true);
	}
}

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT: {
		return 0;
	}
	case WM_CLOSE: {
		EngineCloseEvent e(0, "Window closed, Shutting Down the Engine");
		e.Fire();
		PostQuitMessage(0);
	} break;
	case WM_MOUSEMOVE: {
		InputMouseMoveEvent e(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		e.Fire();
		return 0;
	} break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP: {
		InputMouseButtonEvent e((int)wParam, (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN));
		e.Fire();
		return 0;
	} break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		InputKeyEvent e((int)wParam, (message == WM_KEYDOWN || message == WM_SYSKEYDOWN));
		e.Fire();
		return 0;
	} break;
	case WM_SIZE: {
		WindowResizeEvent e(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		e.Fire();
		return 0;
	} break;

	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif