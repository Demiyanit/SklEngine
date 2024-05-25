#include <renderer/Renderer.hpp>
#include <util/Load.hpp>

Renderer* RendererGet(std::string renderer_dll_path) {
	Renderer* ret = new Renderer();
	Library renderer_dll = LibraryLoader::Load(renderer_dll_path);
	//Import all the functions
	ret->RendererLibrary = renderer_dll;
	ret->Initialize =	renderer_dll.GetFunction<Renderer::InitializeCallback>	("Initialize");
	ret->Resize =		renderer_dll.GetFunction<Renderer::ResizeCallback>		("Resize");
	ret->BeginFrame =	renderer_dll.GetFunction<Renderer::VoidCallback>		("BeginFrame");
	ret->Render =		renderer_dll.GetFunction<Renderer::RenderCallback>		("Render");
	ret->EndFrame =		renderer_dll.GetFunction<Renderer::VoidCallback>		("EndFrame");
	ret->Shutdown =		renderer_dll.GetFunction<Renderer::VoidCallback>		("Shutdown");
	
	return ret;
}
