#pragma once
#ifdef VULKANRENDERER_EXPORTS
#define VULKANRENDERER_API __declspec(dllexport)
#else
#define VULKANRENDERER_API __declspec(dllimport)
#endif

#include <core/Application.hpp>
#include <renderer/Renderer.hpp>
#include <vulkan/vulkan.hpp>
namespace Skl {
VULKANRENDERER_API void Initialize(IApplication* app, void* window_handle);
VULKANRENDERER_API void Resize(int width, int height);
VULKANRENDERER_API void BeginFrame();
VULKANRENDERER_API void Render(RenderData* data);
VULKANRENDERER_API void EndFrame();
VULKANRENDERER_API void ShutDown();

class VULKANRENDERER_API VulkanContext {
public:
    VulkanContext() = default;
    ~VulkanContext() = default;
    vk::Instance instance;
    vk::SurfaceKHR surface;
    class VulkanDevice* main_device = nullptr;

private:
    friend void Initialize(IApplication* app, void* window_handle);
    friend void Resize(int width, int height);
    friend void BeginFrame();
    friend void Render(RenderData* data);
    friend void EndFrame();
    friend void ShutDown();

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;
};

void CreateVulkanSurface(IApplication* app ,void* window_handle);

extern VulkanContext* ctx;
};