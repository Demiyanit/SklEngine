#pragma once

#ifdef SKLC_EXPORT
#define SKLC_API __declspec(dllexport)
#else
#define SKLC_API __declspec(dllimport)
#endif

class IApplication;
class Renderer;

class SKLC_API Engine {
private:
	static IApplication* application_instance;
	static Engine* instance;
	friend class Renderer;
	friend class Event;
	friend Engine* GetEngineInstance();
public:
	Renderer* renderer = nullptr;
	void Initialize(IApplication* inst);
	int  Run();
	void Shutdown();
};