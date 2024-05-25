#pragma once

#include <vector>
#include <string>
#include <core/Application.hpp>
#include <renderer/Shader.hpp>
#include <renderer/ShaderPacket.hpp>
#include <renderer/Model.hpp>
#include <util/Load.hpp>

class RenderData {
public:
	IShaderPacket* shader_packet;
	IModel* model;
};

class Renderer {
public:
	//Keep it cuz BRUH
	Library RendererLibrary;
	using InitializeCallback = void(*)(IApplication* app, void* window_handle);
	using VoidCallback =   void(*)();
	using ResizeCallback = void(*)(int width, int height);
	using RenderCallback = void(*)(RenderData* data);
	void (*Initialize)(IApplication* app, void* window_handle);
	
	void (*Resize)(int width, int height);

	void (*BeginFrame)();
	void (*Render)(RenderData* data);
	void (*EndFrame)();

	//Virtual destructor would work better but BRUH lol
	void (*Shutdown)();
	void* internal_dara = nullptr;
public:
	IModel*			(*LoadModel)(std::string path);
	IShader*		(*LoadAndCompileShader)(std::string path);
	IShaderPacket*	(*ConstructShaderPacket)(std::vector<IShader> shaders);
};

Renderer* RendererGet(std::string renderer_dll_path);