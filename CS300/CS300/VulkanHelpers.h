#ifdef USE_VULKAN
#pragma once
#include <string>
#include <vulkan/vulkan.hpp>

namespace VulkanHelpers
{
	struct ValidationLayersData
	{
		uint32_t m_layerCount = 0;
		std::vector<LayerProperties> m_availableLayers;
		const std::vector<const char*> m_validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};

	struct ExtensionLayersData
	{
		uint32_t m_extensionCount;
		uint32_t m_availableExtensionCount;
	};

	void CreateInstance(const std::string& appName, VkInstance* instance);
	bool CheckLayers(ValidationLayersData& layerData);
	bool GetValidationLayers(ValidationLayersData& layerData);
	bool GetExtensionsLayers(ExtensionLayersData& extensionsData);



}

#endif // USE_VULKAN
