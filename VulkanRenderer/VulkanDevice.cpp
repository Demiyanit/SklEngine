#include <iostream>
#include <iomanip>
#include "VulkanRenderer.h"
namespace Skl {
VulkanDevice::VulkanDevice() {
	
}

VulkanDevice::~VulkanDevice() {

}

vk::PhysicalDevice VulkanDevice::SelectPhysicalDevice() {
	std::vector<vk::PhysicalDevice> PhysicalDevices = ctx->instance.enumeratePhysicalDevices();
	if (PhysicalDevices.size() == 0)
		throw std::runtime_error("No devices which support Vulkan were found.");

	VulkanRequirements req;
	req.ext_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	for (vk::PhysicalDevice PhysicalDevice : PhysicalDevices) {
		vk::PhysicalDeviceProperties2 props2;
		vk::PhysicalDeviceDriverProperties driver_props;
		props2.pNext = &driver_props;
		props2 = PhysicalDevice.getProperties2();
		vk::PhysicalDeviceProperties props = props2.properties;
		vk::PhysicalDeviceFeatures features = PhysicalDevice.getFeatures();
		vk::PhysicalDeviceFeatures2 features2;
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT dynamic_state_next;
		features2.pNext = &dynamic_state_next;
		vk::PhysicalDeviceLineRasterizationFeaturesEXT smooth_line_next;
		dynamic_state_next.pNext = &smooth_line_next;
		features2 = PhysicalDevice.getFeatures2();
		vk::PhysicalDeviceMemoryProperties mem_props;
		mem_props = PhysicalDevice.getMemoryProperties();
#if _DEBUG
		std::cout << "[Renderer]: Evaluating device: " << props.deviceName << ", ID: " << props.deviceID << std::endl;
#endif // _DEBUG

		bool local_host_visible = false;
		for (uint32_t i = 0; i < mem_props.memoryTypeCount; ++i) {
			// Check each memory type to see if its bit is set to 1.
			if (
				((mem_props.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible)) &&
				((mem_props.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal))) {
				local_host_visible = true;
				break;
			}
		}

		VulkanPhysicalDeviceQueueFamilyInfo queue_info;
		bool result = PhysicalDeviceMeetsRequirements(
			PhysicalDevice,
			ctx->surface,
			props,
			features,
			req,
			queue_info,
			this->swapchain_support
		);
		if (result) {
			this->api_major = vk::versionMajor(props.apiVersion);
			this->api_minor = vk::versionMinor(props.apiVersion);
			this->api_patch = vk::versionPatch(props.apiVersion);
#if _DEBUG
			std::cout << "[Renderer]: Selected device: " << props.deviceName << std::endl;
			//Debug GPU info
			switch (props.deviceType) {
			case vk::PhysicalDeviceType::eOther: {
				std::cout << "[Renderer]: Physical device type: Unknown" << std::endl;
			}break;
			//NOTE: For now should NEVER happen
			case vk::PhysicalDeviceType::eIntegratedGpu: {
				std::cout << "[Renderer]: Physical device type: Integrated GPU" << std::endl;
			}break;
			case vk::PhysicalDeviceType::eDiscreteGpu: {
				std::cout << "[Renderer]: Physical device type: Discrete GPU" << std::endl;
			}break;
			case vk::PhysicalDeviceType::eVirtualGpu: {
				std::cout << "[Renderer]: Physical device type: Virtual GPU" << std::endl;
			}break;
			case vk::PhysicalDeviceType::eCpu: {
				//NOTE: I'm allowed to make jokes in my own engine from time to time
				std::cout << "[Renderer]: Physical device type: CPU... Wtf?" << std::endl;
			}break;
			}
			//Driver and vulkan info
			std::cout << "[Renderer]: Device driver info: " << driver_props.driverInfo << std::endl;
			std::cout << "[Renderer]: Vulkan API version: " << this->api_major << "." << "." << api_minor << "." << api_patch << std::endl;
			//Memory info
			for (auto mem : mem_props.memoryHeaps) {
				float mem_size_gib = mem.size / (1024.0f * 1024.0f * 1024.0f);
				if (mem.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
					std::cout << "Local Device memory: " << std::fixed << std::setprecision(3) << mem_size_gib << std::endl;
				} else {
					std::cout << "Shared System memory: " << std::fixed << std::setprecision(3) << mem_size_gib << std::endl;
				}
			}
#endif // _DEBUG
		}
	}
}

bool VulkanDevice::PhysicalDeviceMeetsRequirements(vk::PhysicalDevice device, vk::SurfaceKHR surface, vk::PhysicalDeviceProperties& properties, vk::PhysicalDeviceFeatures& features, VulkanRequirements req, VulkanPhysicalDeviceQueueFamilyInfo& out_queue_info, VulkanSwapchainSupportInfo& out_swapchain_info) {
	out_queue_info.compute_family_index = out_queue_info.graphics_family_index = out_queue_info.present_family_index = -1;
	if (req.discrete_gpu) {
		if (properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu) {
#if _DEBUG
			std::cout << "[Renderer]: Device is not a discrete GPU, and one is required. Skipping." << std::endl;
#endif // _DEBUG
			return false;
		}
	}
	std::vector<vk::QueueFamilyProperties> fam_props = device.getQueueFamilyProperties();
#if _DEBUG
	std::cout << "Graphics | Present | Compute | Transfer | Name" << std::endl;
#endif // _DEBUG
	uint8_t min_score = 255;
	int i = 0;
	for (int i = 0; i < fam_props.size(); i++) {
		uint8_t score = 0;

		//Graphics, should be supported by default ¯\_(ツ)_/¯
		if(out_queue_info.graphics_family_index == -1 && fam_props[i].queueFlags & vk::QueueFlagBits::eGraphics) {
			out_queue_info.graphics_family_index = i;
			++score;
			vk::Bool32 supports_present = vk::False;
			supports_present = device.getSurfaceSupportKHR(i, ctx->surface);
			if (supports_present) {
				out_queue_info.present_family_index = i;
				++score;
			}
		}

		//Compute
		if (fam_props[i].queueFlags & vk::QueueFlagBits::eCompute) {
			out_queue_info.compute_family_index = i;
			++score;
		}

		//Transfer
		if (fam_props[i].queueFlags & vk::QueueFlagBits::eTransfer) {
			// Take the index if it is the current lowest. This increases the
			// liklihood that it is a dedicated transfer queue.
			if (score <= min_score) {
				min_score = score;
				out_queue_info.transfer_family_index = i;
			}
		}

		// If a present queue hasn't been found, iterate again and take the first one.
		// This should only happen if there is a queue that supports graphics but NOT
		// present.
		if (out_queue_info.present_family_index == -1) {
			for (uint32_t i = 0; i < fam_props.size(); ++i) {
				vk::Bool32 supports_present = device.getSurfaceSupportKHR(i, surface);
				if (supports_present) {
					out_queue_info.present_family_index = i;

					// If they differ, bleat about it and move on. This is just here for troubleshooting
					// purposes.
					if (out_queue_info.present_family_index != out_queue_info.graphics_family_index) {
#if _DEBUG
						std::cout << "Warning: Different queue index used for present vs graphics: " << i << std::endl;
#endif
					}
					break;
				}
			}
		}

		std::cout << "[Renderer]: graphics_family_index: " << (out_queue_info.graphics_family_index != -1 ? "yes" : "no") << " | ";
		std::cout << "present_family_index: " << (out_queue_info.present_family_index != -1 ? "yes" : "no") << " | ";
		std::cout << "compute_family_index: " << (out_queue_info.compute_family_index != -1 ? "yes" : "no") << " | ";
		std::cout << "transfer_family_index: " << (out_queue_info.transfer_family_index != -1 ? "yes" : "no") << " | ";
		std::cout << "deviceName: " << properties.deviceName << std::endl;
	
		if (
			(!req.graphics || (req.graphics && out_queue_info.graphics_family_index != -1)) &&
			(!req.present || (req.present && out_queue_info.present_family_index != -1)) &&
			(!req.compute || (req.compute && out_queue_info.compute_family_index != -1)) &&
			(!req.transfer || (req.transfer && out_queue_info.transfer_family_index != -1))) {
#ifdef _DEBUG
			std::cout << "[Renderer]: Device meets queue requirements." << std::endl;
			std::cout << "Graphics Family Index: " << out_queue_info.graphics_family_index << std::endl;
			std::cout << "Present Family Index: " << out_queue_info.present_family_index << std::endl;
			std::cout << "Transfer Family Index: " << out_queue_info.transfer_family_index << std::endl;
			std::cout << "Compute Family Index: " << out_queue_info.compute_family_index << std::endl;
#endif // _DEBUG

			//Query swapchain support
			QuerySwapChainSupport(device, surface, out_swapchain_info);
			if (out_swapchain_info.formats.size() < 1 || out_swapchain_info.present_modes.size() < 1) {
#ifdef _DEBUG
				std::cout << "[Renderer]: Required swapchain support not present, skipping device." << std::endl;
#endif // _DEBUG
				return false;
			}

			//Device extensions
			if (req.ext_names.size() > 0) {
				auto ext_props = device.enumerateDeviceExtensionProperties();
				for (auto ext : req.ext_names) {
					bool found = false;
					for (auto av_ext : ext_props) {
						if (av_ext.extensionName == ext) {
							found = true;
							break;
						}
					}
					if (!found) {
#if _DEBUG
						std::cout << "[Renderer]: Required extension not found: " << ext << ", skipping device" << std::endl;
#endif // _DEBUG
						return false;
					}
				}
			}
		}
		if (req.sampler_anisotropy && !features.samplerAnisotropy) {
#if _DEBUG
			std::cout << "[Renderer]: Device doesn't support sampler anisotrophy, skipping..." << std::endl;
#endif // _DEBUG
			return false;
		}
		return true;
	}
}
void Skl::VulkanDevice::QuerySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface, VulkanSwapchainSupportInfo& info) {
	info.caps = device.getSurfaceCapabilitiesKHR(surface);
	info.formats = device.getSurfaceFormatsKHR(surface);
	info.present_modes = device.getSurfacePresentModesKHR(surface);
}





};//namespace Skl