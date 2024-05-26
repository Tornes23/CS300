#ifdef USE_VULKAN
#include <stdexcept>
#include <iostream>
#include <CustomDebug/VulkanDebug.h>
#include "VulkanHelpers.h"
#include "VulkanHelpers.h"
#include "VulkanHelpers.h"

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

	bool CheckExtensions(ExtensionLayersData& extensionsData)
	{
		/* Look for instance extensions */
		vk::Bool32 surfaceExtFound = VK_FALSE;
		vk::Bool32 platformSurfaceExtFound = VK_FALSE;

		VkResult result = result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsData.m_extensionCount, extensionsData.m_availableExtensions.data());
		VERIFY(result == VkResult::VK_SUCCESS);

		if (extensionsData.m_extensionCount > 0)
		{
			for (uint32_t i = 0; i < extensionsData.m_extensionCount; i++)
			{
				if (!strcmp(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, extensionsData.m_availableExtensions[i].extensionName))
				{
					extensionsData.m_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
				}
				if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, extensionsData.m_availableExtensions[i].extensionName))
				{
					surfaceExtFound = 1;
					extensionsData.m_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
				}

				if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, extensionsData.m_availableExtensions[i].extensionName))
				{
					platformSurfaceExtFound = 1;
					extensionsData.m_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
				}

				assert(extensionsData.m_extensionCount < 64);
			}
		}

		if (!surfaceExtFound)
		{
			ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_SURFACE_EXTENSION_NAME
				" extension.\n\n"
				"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
				"Please look at the Getting Started guide for additional information.\n",
				"vkCreateInstance Failure");
		}

		if (!platformSurfaceExtFound)
		{
			ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find the " VK_KHR_WIN32_SURFACE_EXTENSION_NAME
				" extension.\n\n"
				"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
				"Please look at the Getting Started guide for additional information.\n",
				"vkCreateInstance Failure");
		}

#ifndef NDEBUG
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

	bool GetExtensionsLayers(ExtensionLayersData& extensionsData)
	{
		return CheckExtensions(extensionsData);
	}


	void CreateInstance(const std::string& appName, VkInstance* instance)
	{
		ValidationLayersData layersData;
		layersData.m_layerCount = 0;
		ExtensionLayersData extensionsData;
		extensionsData.m_extensionCount = 0;

#ifndef NDEBUG
		if (GetValidationLayers(layersData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN INSTANCE CREATION] Validation layers requested, but not available!");
		}
#endif

		if (GetExtensionsLayers(extensionsData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN INSTANCE CREATION] Extensions requested, but not available!");
		}

		auto const app = vk::ApplicationInfo()
			.setPApplicationName(appName.c_str())
			.setApplicationVersion(0)
			.setPEngineName(appName.c_str())
			.setEngineVersion(0)
			.setApiVersion(VK_API_VERSION_1_0);

		const VkInstanceCreateInfo inst_info = vk::InstanceCreateInfo()
			.setPApplicationInfo(&app)
			.setEnabledLayerCount(layersData.m_layerCount)
#ifndef NDEBUG
			.setPpEnabledLayerNames(layersData.m_validationLayers.data())
#endif
			.setEnabledExtensionCount(extensionsData.m_extensionCount)
			.setPpEnabledExtensionNames(extensionsData.m_extensions.data());

		VkResult result = vkCreateInstance(&inst_info, nullptr, instance);

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

#ifndef NDEBUG
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
		m_createInfo.pUserData = nullptr; // Optional
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
