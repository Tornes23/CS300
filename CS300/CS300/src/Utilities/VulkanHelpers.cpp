#ifdef USE_VULKAN
#include <stdexcept>
#include <iostream>
#include <CustomDebug/VulkanDebug.h>
#include "VulkanHelpers.h"
#include <SDL_vulkan.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define VK_KHR_win32_surface 1
#define VK_KHR_WIN32_SURFACE_SPEC_VERSION 6
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

namespace VulkanHelpers
{
	bool CheckLayers(ValidationLayersData& layersData)
	{
		for (const char* layerName : layersData.m_validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : layersData.m_availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	bool CheckExtensions(ExtensionLayersData& extensionsData, SDL_Window* window)
	{
		/* Look for instance extensions */
		if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionsData.m_extensionCount, nullptr))
		{
			return false;
		}
		extensionsData.m_extensions.resize(extensionsData.m_extensionCount);
		SDL_Vulkan_GetInstanceExtensions(window, &extensionsData.m_extensionCount, extensionsData.m_extensions.data());
#ifdef DEBUG
		extensionsData.m_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		extensionsData.m_extensionCount++;
#endif

		return true;
	}

	bool GetValidationLayers(ValidationLayersData& layersData)
	{
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, nullptr);
		layersData.m_availableLayers.resize(layersData.m_layerCount);
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, layersData.m_availableLayers.data());

		return CheckLayers(layersData);
	}

	bool GetExtensionsLayers(ExtensionLayersData& extensionsData, SDL_Window* window)
	{
		return CheckExtensions(extensionsData, window);
	}


	void CreateInstance(const std::string& appName, VkInstance* instance, SDL_Window* window)
	{
		ValidationLayersData layersData;
		layersData.m_layerCount = 0;
		ExtensionLayersData extensionsData;
		extensionsData.m_extensionCount = 0;

#ifdef DEBUG
		if (!GetValidationLayers(layersData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN INSTANCE CREATION] Validation layers requested, but not available!");
		}
#endif

		if (!GetExtensionsLayers(extensionsData, window))
		{
			//throw exception
			throw std::runtime_error("[VULKAN INSTANCE CREATION] Extensions requested, but not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = appName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

#ifdef DEBUG
		VulkanHelpers::DebugCallbackData debugCallbackData;
		debugCallbackData.Initialize();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = debugCallbackData.GetCreateInfo();
#endif

		VkInstanceCreateInfo createinfo{};
		createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createinfo.pApplicationInfo = &appInfo;
#ifdef DEBUG
		createinfo.enabledLayerCount = (uint32_t)layersData.m_validationLayers.size();
		createinfo.ppEnabledLayerNames  = layersData.m_validationLayers.data();
		createinfo.pNext = &debugCreateInfo;
#endif
		createinfo.enabledExtensionCount = (uint32_t)extensionsData.m_extensions.size();
		createinfo.enabledExtensionCount = extensionsData.m_extensionCount;
		createinfo.ppEnabledExtensionNames = extensionsData.m_extensions.data();

		VkResult result = vkCreateInstance(&createinfo, nullptr, instance);

		if (result == VkResult::VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			ERR_EXIT(
				"Cannot find a compatible Vulkan installable client driver (ICD).\n\n"
				"Please look at the Getting Started guide for additional information.\n",
				"vkCreateInstance Failure");
		}
		else if (result == VkResult::VK_ERROR_EXTENSION_NOT_PRESENT)
		{
			ERR_EXIT(
				"Cannot find a specified extension library.\n"
				"Make sure your layers path is set appropriately.\n",
				"vkCreateInstance Failure");
		}
		else if (result != VkResult::VK_SUCCESS)
		{
			ERR_EXIT(
				"vkCreateInstance failed.\n\n"
				"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
				"Please look at the Getting Started guide for additional information.\n",
				"vkCreateInstance Failure");
		}
	}

	void DestroyInstance(VkInstance* instance)
	{
		vkDestroyInstance(*instance, nullptr);
	}

	bool GetGPUDevice()
	{

	}

#ifdef DEBUG
	VkResult CreateDebugCallback(DebugCallbackData& debugData)
	{
		
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*debugData.m_instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(*debugData.m_instance, &debugData.m_createInfo, debugData.m_pAllocator, debugData.m_pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
		
	}

	void DestroyDebugCallback(DebugCallbackData& debugData)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*debugData.m_instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) 
		{
			func(*debugData.m_instance, *debugData.m_pDebugMessenger, debugData.m_pAllocator);
		}
	}

	void DebugCallbackData::Initialize(VkInstance* instance,
									   const VkAllocationCallbacks* pAllocator,
									   VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		m_instance = instance;
		m_pAllocator = pAllocator;
		m_pDebugMessenger = pDebugMessenger;
		PopulateCreateinfo();
	}

	VkDebugUtilsMessengerCreateInfoEXT DebugCallbackData::GetCreateInfo() const
	{
		return m_createInfo;
	}

	void DebugCallbackData::PopulateCreateinfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		m_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		m_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		m_createInfo.pfnUserCallback = debugCallback;
	}

	void DebugCallbackData::Initialize()
	{
		PopulateCreateinfo();
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
												 VkDebugUtilsMessageTypeFlagsEXT messageType, 
												 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
												 void* pUserData)
	{
	
		ActualDebugPrint(messageSeverity, messageType, pCallbackData, pUserData);
	
		return VK_FALSE;
	}

#endif

}
#endif // USE_VULKAN
