#ifdef USE_VULKAN
#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_TYPESAFE_CONVERSION

#ifndef NDEBUG
#define VERIFY(x) assert(x)
#else
#define VERIFY(x) ((void)(x))
#endif

#include <windows.h>
#include <vulkan/vulkan.hpp>


#define ERR_EXIT(err_msg, err_class)                        \
    do                                                      \
    {                                                       \
        MessageBox(nullptr, err_msg, err_class, MB_OK);     \
        exit(1);                                            \
    } while (0)

namespace VulkanHelpers
{


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

    void ActualDebugPrint(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                          const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
                          void* pUserData);
}

#endif // USE_VULKAN
