#include "VulkanRenderer.h"

#include <iostream>
#include <cstdint>

void Initialize(IApplication* app) {
#if _DEBUG
	std::cout << "[Renderer]: Vulkan Renderer [DEBUG VERSION]" << std::endl;
#endif // _DEBUG

	if (app == nullptr) 
		throw new std::runtime_error("Tried to initialize renderer with NULL application instance");
	
	uint32_t api_version = vk::enumerateInstanceVersion();
	
	VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	app_info.apiVersion = api_version;
	//TODO: Update
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
	app_info.engineVersion		= VK_MAKE_API_VERSION(0, 0, 0, 1);
	app_info.pApplicationName   = app->ApplicationName.c_str();
	app_info.pEngineName		= "SklEngine";
	
	VkInstanceCreateInfo instance_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	instance_info.pApplicationInfo = &app_info;

	std::vector<const char*> required_extensions;
	required_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if _DEBUG
	required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	std::cout << "[Renderer]: Required Extensions:" << std::endl;
	for (const char* ext : required_extensions) {
		std::cout << "[Renderer]: " << ext << std::endl;
	}
#endif // _DEBUG

	instance_info.enabledExtensionCount  = required_extensions.size();
	instance_info.ppEnabledExtensionNames = required_extensions.data();

	std::vector<vk::ExtensionProperties> extension_props = vk::enumerateInstanceExtensionProperties(nullptr, nullptr);
	if (extension_props.size() == 0) 
		throw new std::runtime_error("Could not retrieve the list of Vulkan extension properties");
#if _DEBUG
	std::cout << "[Renderer]: Discovered Vulkan extensions:" << std::endl;
	for (const VkExtensionProperties& extension : extension_props) {
		std::cout << "[Renderer]: " << extension.extensionName << std::endl;
	}
#endif // _DEBUG

	for (uint32_t i = 0; i < required_extensions.size(); ++i) {
		bool found = false;
		for (uint32_t j = 0; j < extension_props.size(); ++j) {
			if (std::string(required_extensions[i]) == extension_props[j].extensionName) {
				found = true;
#if _DEBUG
				std::cout << "Required extension found: " << required_extensions[i] << std::endl;
#endif // _DEBUG
				break;
			}
		}
		if (!found) {
			throw new std::runtime_error("Required extension is missing: " + std::string(required_extensions[i]));
		}
	}

	VkInstance instance = vk::createInstance(instance_info);
}

void Resize(int width, int height) {

}

void BeginFrame() {

}

void Render(RenderData* data) {

}

void EndFrame() {

}

void ShutDown() {

}
