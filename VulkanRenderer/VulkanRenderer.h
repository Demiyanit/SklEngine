#pragma once
#ifdef VULKANRENDERER_EXPORTS
#define VULKANRENDERER_API __declspec(dllexport)
#else
#define VULKANRENDERER_API __declspec(dllimport)
#endif

#include <core/Application.hpp>
#include <renderer/Renderer.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vk_enum_string_helper.h>

VULKANRENDERER_API void Initialize(IApplication* app);
VULKANRENDERER_API void Resize(int width, int height);
VULKANRENDERER_API void BeginFrame();
VULKANRENDERER_API void Render(RenderData* data);
VULKANRENDERER_API void EndFrame();
VULKANRENDERER_API void ShutDown();
