#if defined(_WIN32) || defined(_WIN64)

#include "VulkanDevice.h"
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
namespace Skl {
void CreateVulkanSurface(IApplication* app, void* window_handle) {
	HWND hwnd = (HWND)window_handle;
	VkSurfaceKHR _surf;
	VkWin32SurfaceCreateInfoKHR surface_info;
	surface_info.hwnd = hwnd;
	
	VkResult res = vkCreateWin32SurfaceKHR(ctx->instance, &surface_info, nullptr, &_surf);
	if (res != VK_SUCCESS)
		throw new std::runtime_error("Failed to create Vulkan surface");
	ctx->surface = vk::SurfaceKHR(_surf);
}
#endif // WIN32
};