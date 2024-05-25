#ifdef USE_VULKAN
#include <stdexcept>
#include <iostream>
#include "VulkanHelpers.h"
#include <CustomDebug/VulkanDebug.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define VK_KHR_win32_surface 1
#define VK_KHR_WIN32_SURFACE_SPEC_VERSION 6
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

namespace VulkanHelpers
{
	unsigned int CheckLayers(uint32_t check_count, char const* const* const check_names, uint32_t layer_count,
		vk::LayerProperties* layers)
	{
		for (uint32_t i = 0; i < check_count; i++)
		{
			unsigned int found = VK_FALSE;
			for (uint32_t j = 0; j < layer_count; j++)
			{
				std::string name = layers[j].layerName.data();
				std::cout << name << std::endl;
				if (!strcmp(check_names[i], name.c_str()))
				{
					found = VK_TRUE;
					break;
				}
			}
			if (!found)
			{
				fprintf(stderr, "Cannot find layer: %s\n", check_names[i]);
				return 0;
			}
		}
		return VK_TRUE;
	}


	void CreateInstance(const std::string& appName, VkInstance* instance)
	{
		uint32_t           instance_extension_count = 0;
		uint32_t           instance_layer_count = 0;
		char const* const instance_validation_layers[] = { "VK_LAYER_KHRONOS_validation" };
		char const* extension_names[64];
		char const* enabled_layers[64];
		unsigned int enabled_extension_count = 0;
		unsigned int enabled_layer_count = 0;

		// Look for validation layers
		unsigned int validation_found = VK_FALSE;
		auto result = vk::enumerateInstanceLayerProperties(&instance_layer_count, static_cast<vk::LayerProperties*>(nullptr));
		if (result == vk::Result::eSuccess)
		{

			if (instance_layer_count > 0)
			{
				std::unique_ptr<vk::LayerProperties[]> instance_layers(new vk::LayerProperties[instance_layer_count]);
				result = vk::enumerateInstanceLayerProperties(&instance_layer_count, instance_layers.get());
				VERIFY(result == vk::Result::eSuccess);

				validation_found = CheckLayers(ARRAY_SIZE(instance_validation_layers), instance_validation_layers,
					instance_layer_count, instance_layers.get());
				if (validation_found)
				{
					enabled_layer_count = ARRAY_SIZE(instance_validation_layers);
					enabled_layers[0] = "VK_LAYER_KHRONOS_validation";
				}
			}

			if (!validation_found)
			{
				ERR_EXIT(
					"vkEnumerateInstanceLayerProperties failed to find required validation layer.\n\n"
					"Please look at the Getting Started guide for additional information.\n",
					"vkCreateInstance Failure");
			}
		}

		/* Look for instance extensions */
		unsigned int surfaceExtFound = VK_FALSE;
		unsigned int platformSurfaceExtFound = VK_FALSE;
		memset(extension_names, 0, sizeof(extension_names));

		result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count,
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
		auto const app = vk::ApplicationInfo()
			.setPApplicationName(appName.c_str())
			.setApplicationVersion(0)
			.setPEngineName(appName.c_str())
			.setEngineVersion(0)
			.setApiVersion(VK_API_VERSION_1_0);

		const VkInstanceCreateInfo inst_info = vk::InstanceCreateInfo()
			.setPApplicationInfo(&app)
			.setEnabledLayerCount(enabled_layer_count)
			.setPpEnabledLayerNames(instance_validation_layers)
			.setEnabledExtensionCount(enabled_extension_count)
			.setPpEnabledExtensionNames(extension_names);

		if (vkCreateInstance(&inst_info, nullptr, instance) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create instance!");
		}
	}
}
#endif // USE_VULKAN
