#pragma once

#include "VulkanRenderer.h"
namespace Skl {
class VulkanDevice final {
public:
	VulkanDevice();
	~VulkanDevice();
private:
	struct VulkanRequirements {
		bool graphics = true;
		bool present = true;
		bool transfer = true;
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
	vk::PhysicalDevice SelectPhysicalDevice();
	bool PhysicalDeviceMeetsRequirements(
		vk::PhysicalDevice device, 
		vk::SurfaceKHR surface,
		vk::PhysicalDeviceProperties& properties,
		vk::PhysicalDeviceFeatures  & features,
		VulkanRequirements req,
		VulkanPhysicalDeviceQueueFamilyInfo& out_queue_info,
		VulkanSwapchainSupportInfo& out_swapchain_info
		);
};
};