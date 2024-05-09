#if defined(_WIN32) && defined(_WIN64)
#include <stdexcept>

#include <glew/glew.h>
#include <glew/wglew.h>

#include <platform/Window.hpp>
#include <renderer/OpenGLRenderer.hpp>

void OpenGLRenderer::InitGlew() {
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = 0;
	wc.lpfnWndProc = DefWindowProcW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"DummyWndClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassExW(&wc);


	HWND hwnd = CreateWindowEx(
		0,
		L"DummyWndClass",
		L"Dummy Window",
		WS_POPUP | WS_DISABLED,
		0, 0, 1, 1,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);

	if (hwnd == NULL) {
		throw std::runtime_error("CreateWindowEx failed");
	}

	HDC hdc = GetDC(hwnd);

	if (hdc == NULL) {
		throw std::runtime_error("GetDC failed");
	}

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int format = ChoosePixelFormat(hdc, &pfd);

	if (format == 0) {
		throw std::runtime_error("ChoosePixelFormat failed");
	}

	if (SetPixelFormat(hdc, format, &pfd) == FALSE) {
		throw std::runtime_error("SetPixelFormat failed");
	}

	HGLRC hglrc = wglCreateContext(hdc);

	if (hglrc == NULL) {
		throw std::runtime_error("wglCreateContext failed");
	}

	if (wglMakeCurrent(hdc, hglrc) == FALSE) {
		throw std::runtime_error("wglMakeCurrent failed");
	}

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();

	if (err != GLEW_OK) {
		throw std::runtime_error((const char*)glewGetErrorString(err));
	}

	if (wglMakeCurrent(NULL, NULL) == FALSE) {
		throw std::runtime_error("wglMakeCurrent failed");
	}

	if (wglDeleteContext(hglrc) == FALSE) {
		throw std::runtime_error("wglDeleteContext failed");
	}

	if (DestroyWindow(hwnd) == 0) {
		throw std::runtime_error("DestroyWindow failed");
	}
	UnregisterClassW(L"DummyWndClass", GetModuleHandleW(NULL));
	InitMain();
}

void OpenGLRenderer::InitMain() {
	HDC hdc = GetDC((HWND)Window::wnd_handle);

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int format = ChoosePixelFormat(hdc, &pfd);

	if (format == 0) {
		throw std::runtime_error("ChoosePixelFormat failed");
	}

	if (SetPixelFormat(hdc, format, &pfd) == FALSE) {
		throw std::runtime_error("SetPixelFormat failed");
	}

	ctx = wglCreateContextAttribsARB(hdc, NULL, 0);

	if (ctx == NULL) {
		throw std::runtime_error("wglCreateContextAttribsARB failed");
	}

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	ctx = wglCreateContextAttribsARB(hdc, NULL, attribs);

	if (ctx == NULL) {
		throw std::runtime_error("wglCreateContextAttribsARB failed");
	}

	if (wglMakeCurrent(hdc, (HGLRC)ctx) == FALSE) {
		throw std::runtime_error("wglMakeCurrent failed");
	}
}

void OpenGLRenderer::SwapBuffers() {
	wglSwapLayerBuffers(GetDC((HWND)Window::wnd_handle), WGL_SWAP_MAIN_PLANE);
}

void OpenGLRenderer::MakeCtxCurrent() {
	if (wglMakeCurrent(GetDC((HWND)Window::wnd_handle), (HGLRC)ctx) == FALSE) {
		throw std::runtime_error("wglMakeCurrent failed");
	}
}

#endif