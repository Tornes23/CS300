#ifdef USE_VULKAN
#pragma once
#include <string>

struct  VkInstance;
struct  LayerProperties;

namespace VulkanHelpers
{
	void CreateInstance(const std::string& appName, VkInstance* instance);
	unsigned int CheckLayers(uint32_t, const char* const*, uint32_t, vk::LayerProperties*);

}

#endif // USE_VULKAN
