#include "VulkanRenderer.h"
#include <iostream>
#include <cstdint>
namespace Skl {
VulkanContext* ctx;

VkBool32 SklEngineVkCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageTypes,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData) {
  std::cerr << "Vulkan Error: " << pCallbackData->pMessage << std::endl;
  return VK_TRUE;
}

void Initialize(IApplication* app, void* window_handle) {
#if _DEBUG
	std::cout << "[Renderer]: Vulkan Renderer [DEBUG VERSION]" << std::endl;
#endif // _DEBUG

	if (app == nullptr) 
		throw new std::runtime_error("Tried to initialize renderer with NULL application instance");
	uint32_t api_version = vk::enumerateInstanceVersion();
	
	vk::ApplicationInfo app_info;
	app_info.apiVersion = api_version;
	//TODO: Update
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
	app_info.engineVersion		= VK_MAKE_API_VERSION(0, 0, 0, 1);
	app_info.pApplicationName   = app->ApplicationName.c_str();
	app_info.pEngineName		= "SklEngine";
	
	vk::InstanceCreateInfo instance_info;
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

	std::vector<vk::ExtensionProperties> extension_props = vk::enumerateInstanceExtensionProperties();
	if (extension_props.size() == 0) 
		throw new std::runtime_error("Could not retrieve the list of Vulkan extension properties");
#if _DEBUG
	std::cout << "[Renderer]: Discovered Vulkan extensions:" << std::endl;
	for (const VkExtensionProperties& extension : extension_props) {
		std::cout << "[Renderer]: " << extension.extensionName << std::endl;
	}
#endif // _DEBUG

	for (const char* ext : required_extensions) {
		bool found = false;
		std::string ext_str = ext;
		for (vk::ExtensionProperties ext_prop : extension_props) {
			if (ext_str == ext_prop.extensionName) {
				found = true;
#if _DEBUG
				std::cout << "[Renderer]: Required extension found: " << ext_str << std::endl;
#endif // _DEBUG
				break;
			}
		}
		if (!found) {
			throw new std::runtime_error("Required extension is missing: " + ext_str);
		}
	}

#if _DEBUG
	std::cout << "[Renderer]: Getting validation layers" << std::endl;
	std::vector<const char*> reqiured_layers;
	reqiured_layers.push_back("VK_LAYER_KHRONOS_validation");
	reqiured_layers.push_back("VK_LAYER_LUNARG_api_dump");
	std::vector<vk::LayerProperties> layer_props = vk::enumerateInstanceLayerProperties();
	for (const char* layer : reqiured_layers) {
		bool found = false;
		std::string layer_str = layer;
		for (vk::LayerProperties layer_prop : layer_props) {
			if (layer_str == layer_prop.layerName) {
				found = true;
				std::cout << "[Renderer]: Required layer found: " << layer_str << std::endl;
			}
		}
		if (!found) {
			throw new std::runtime_error("Required layer is missing: " + layer_str);
		}
	}

	instance_info.enabledLayerCount = reqiured_layers.size();
	instance_info.ppEnabledLayerNames = reqiured_layers.data();
#endif // _DEBUG

#if _DEBUG
	std::cout << "[Renderer]: Creating instance..." << std::endl;
#endif
	vk::Instance instance = vk::createInstance(instance_info);
#if _DEBUG
	std::cout << "[Renderer]: Instance created successfully" << std::endl;
#endif
	
#if _DEBUG
	std::cout << "[Renderer]: Initializing Vulkan logger..." << std::endl;
	vk::DebugUtilsMessengerCreateInfoEXT DutilMsgInfo;
	vk::DebugUtilsMessageSeverityFlagsEXT MessageFlags;
	MessageFlags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
	MessageFlags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
	MessageFlags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
	MessageFlags |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
	DutilMsgInfo.messageSeverity = MessageFlags;
	DutilMsgInfo.messageType =
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding;
	DutilMsgInfo.pfnUserCallback = SklEngineVkCallback;
	
	ctx->DebugMessenger = instance.createDebugUtilsMessengerEXTUnique(DutilMsgInfo, nullptr, vk::DispatchLoaderDynamic());
	std::cout << "[Renderer]: Vulkan logger initialized successfully" << std::endl;
#endif // _DEBUG
	ctx = new VulkanContext();
	CreateVulkanSurface(app, window_handle);
	ctx->main_device = new VulkanDevice();
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
};