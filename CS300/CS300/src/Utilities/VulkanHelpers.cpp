#ifdef USE_VULKAN
#include <stdexcept>
#include <iostream>
#include <CustomDebug/VulkanDebug.h>
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
		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
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

	bool GetValidationLayers(ValidationLayersData& layersData)
	{
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, nullptr);
		layersData.m_availableLayers.resize(layersData.m_layerCount);
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, layersData.m_availableLayers.data());

		return CheckLayers(layersData);
	}

	bool GetExtensionsLayers(ExtensionLayersData& extensionsData)
	{
		/* Look for instance extensions */
		vk::Bool32 surfaceExtFound = VK_FALSE;
		vk::Bool32 platformSurfaceExtFound = VK_FALSE;
		memset(extension_names, 0, sizeof(extension_names));

		auto result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count,
			static_cast<vk::ExtensionProperties*>(nullptr));
		VERIFY(result == vk::Result::eSuccess);

		if (instance_extension_count > 0)
		{
			std::unique_ptr<vk::ExtensionProperties[]> instance_extensions(new vk::ExtensionProperties[instance_extension_count]);
			result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count, instance_extensions.get());
			VERIFY(result == vk::Result::eSuccess);

			for (uint32_t i = 0; i < instance_extension_count; i++)
			{
				if (!strcmp(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, instance_extensions[i].extensionName))
				{
					extension_names[enabled_extension_count++] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
				}
				if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName))
				{
					surfaceExtFound = 1;
					extension_names[enabled_extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;
				}

				if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName))
				{
					platformSurfaceExtFound = 1;
					extension_names[enabled_extension_count++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
				}

				assert(enabled_extension_count < 64);
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
		return false;
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
			.setPpEnabledLayerNames(layersData.m_validationLayers)
#endif
			.setEnabledExtensionCount(enabled_extension_count)
			.setPpEnabledExtensionNames(extension_names);

		auto result = vkCreateInstance(&inst_info, nullptr, instance) != VK_SUCCESS) 

		if (result == vk::Result::eErrorIncompatibleDriver)
		{
			ERR_EXIT(
				"Cannot find a compatible Vulkan installable client driver (ICD).\n\n"
				"Please look at the Getting Started guide for additional information.\n",
				"vkCreateInstance Failure");
		}
		else if (result == vk::Result::eErrorExtensionNotPresent)
		{
			ERR_EXIT(
				"Cannot find a specified extension library.\n"
				"Make sure your layers path is set appropriately.\n",
				"vkCreateInstance Failure");
		}
		else if (result != vk::Result::eSuccess)
		{
			ERR_EXIT(
				"vkCreateInstance failed.\n\n"
				"Do you have a compatible Vulkan installable client driver (ICD) installed?\n"
				"Please look at the Getting Started guide for additional information.\n",
				"vkCreateInstance Failure");
		}
	}
}
#endif // USE_VULKAN
