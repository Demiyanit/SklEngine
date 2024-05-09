#pragma once

#ifdef SKLC_EXPORT
#define SKLC_API __declspec(dllexport)
#else
#define SKLC_API __declspec(dllimport)
#endif

class IApplication;

class SKLC_API Engine {
private:
	IApplication* application_instance = nullptr;
public:
	Engine(IApplication* inst);
	int run();
	~Engine();
};
