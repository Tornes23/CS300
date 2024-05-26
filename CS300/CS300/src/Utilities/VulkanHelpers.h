#ifdef USE_VULKAN
#pragma once
#include <string>
#include <vulkan/vulkan.hpp>

namespace VulkanHelpers
{
	struct ValidationLayersData
	{
		uint32_t m_layerCount = 0;
		std::vector<VkLayerProperties> m_availableLayers;
		const std::vector<const char*> m_validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};

	struct ExtensionLayersData
	{
		uint32_t m_extensionCount;
		std::vector<VkExtensionProperties> m_availableExtensions;
		std::vector<const char*> m_extensions;

	};

#ifndef NDEBUG
	struct DebugCallbackData
	{
		VkInstance* m_instance;
		const VkAllocationCallbacks* m_pAllocator;
		VkDebugUtilsMessengerEXT* m_pDebugMessenger;
		VkDebugUtilsMessengerCreateInfoEXT m_createInfo;

		VkDebugUtilsMessengerCreateInfoEXT GetCreateInfo() const;
		void PopulateCreateinfo();
		void Initialize();
		void Initialize(VkInstance* instance,
						const VkAllocationCallbacks* pAllocator,
						VkDebugUtilsMessengerEXT* pDebugMessenger);
	};
#endif

	void CreateInstance(const std::string& appName, VkInstance* instance);
	void DestroyInstance(VkInstance* instance);
#ifndef NDEBUG
	VkResult CreateDebugCallback(DebugCallbackData& debugData);
	void DestroyDebugCallback(DebugCallbackData& debugData);
#endif
	bool CheckLayers(ValidationLayersData& layerData);
	bool CheckExtensions(ExtensionLayersData& extensionsData);
	bool GetValidationLayers(ValidationLayersData& layerData);
	bool GetExtensionsLayers(ExtensionLayersData& extensionsData);



}

#endif // USE_VULKAN
