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

void CreateVulkanSurface(IApplication* app ,void* window_handle);

struct VulkanRequirements {
	bool graphics = true;
	bool present = true;
	bool transfer = true;
	bool compute = true;
	bool sampler_anisotropy = true;
	bool discrete_gpu = true;
	std::vector<const char*> ext_names;
};
struct VulkanPhysicalDeviceQueueFamilyInfo {
	int32_t graphics_family_index;
	int32_t present_family_index;
	int32_t compute_family_index;
	int32_t transfer_family_index;
};
struct VulkanSwapchainSupportInfo {
	vk::SurfaceCapabilitiesKHR caps;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR>   present_modes;
};
class VulkanDevice final {
public:
	uint32_t api_major;
	uint32_t api_minor;
	uint32_t api_patch;
	vk::Device handle;
	VulkanSwapchainSupportInfo swapchain_support;
private:

	vk::PhysicalDevice SelectPhysicalDevice();
	bool PhysicalDeviceMeetsRequirements(
		vk::PhysicalDevice device,
		vk::SurfaceKHR surface,
		vk::PhysicalDeviceProperties& properties,
		vk::PhysicalDeviceFeatures& features,
		VulkanRequirements req,
		VulkanPhysicalDeviceQueueFamilyInfo& out_queue_info,
		VulkanSwapchainSupportInfo& out_swapchain_info
	);
	void QuerySwapChainSupport(
		vk::PhysicalDevice device,
		vk::SurfaceKHR surface,
		VulkanSwapchainSupportInfo& info
	);
};
class VulkanSwapChain {
public: 
	
};
class VulkanContext {
public:
	VulkanContext() = default;
	~VulkanContext() = default;
	vk::Instance instance;
	vk::SurfaceKHR surface;
	class VulkanDevice* main_device = nullptr;
#if _DEBUG
	vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic> DebugMessenger;
#endif // _DEBUG
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
extern VulkanContext* ctx;
};