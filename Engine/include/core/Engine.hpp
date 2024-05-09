#pragma once

#ifdef SKLC_EXPORT
#define SKLC_API __declspec(dllexport)
#else
#define SKLC_API __declspec(dllimport)
#endif

class IApplication;
class IRenderer;

class SKLC_API Engine {
private:
	static IApplication* application_instance;
	static IRenderer* renderer;
	friend class IRenderer;
	friend class Event;
public:

	void Initialize(IApplication* inst);
	int  Run();
	void Shutdown();
};
