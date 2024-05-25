#ifdef USE_VULKAN
#pragma once
#include <string>
#include <vulkan/vulkan.hpp>

namespace VulkanHelpers
{
	void CreateInstance(const std::string& appName, VkInstance* instance);
}

#endif // USE_VULKAN
