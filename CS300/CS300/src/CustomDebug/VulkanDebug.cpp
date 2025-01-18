#ifdef USE_VULKAN
#include <iostream>
#include "VulkanDebug.h"
namespace VulkanHelpers
{

/*
The first parameter specifies the severity of the message, which is one of the following flags:

VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: Diagnostic message
VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: Informational message like the creation of a resource
VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: Message about behavior that is not necessarily an error, but very likely a bug in your application
VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: Message about behavior that is invalid and may cause crashes

The messageType parameter can have the following values:

VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: Some event has happened that is unrelated to the specification or performance
VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: Something has happened that violates the specification or indicates a possible mistake
VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: Potential non-optimal use of Vulkan

The pCallbackData parameter refers to a VkDebugUtilsMessengerCallbackDataEXT struct containing the details of the message 
itself, with the most important members being:
pMessage: The debug message as a null-terminated string
pObjects: Array of Vulkan object handles related to the message
objectCount: Number of objects in array

Finally, the pUserData parameter contains a pointer that was specified during the setup of the callback and allows you to
pass your own data to it.

The callback returns a boolean that indicates if the Vulkan call that triggered the validation layer message should be aborted.
If the callback returns true, then the call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error. This is normally only used 
to test the validation layers themselves, so you should always return VK_FALSE.

All that remains now is telling Vulkan about the callback function. Perhaps somewhat surprisingly, even the debug callback in 
Vulkan is managed with a handle that needs to be explicitly created and destroyed. Such a callback is part of a debug messenger 
and you can have as many of them as you want.
*/
	void ActualDebugPrint(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			// Message is important enough to show
		}
		
		std::cerr << "\nValidation layer: " << pCallbackData->pMessage << std::endl << std::endl;
	}
}
#endif
