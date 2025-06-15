#ifdef USE_VULKAN
#include <limits>
#include <stdexcept>
#include <set>
#include <iostream>
#include <algorithm>
#include <CustomDebug/VulkanDebug.h>
#include <vulkan/vk_sdk_platform.h>
#include "VulkanHelpers.h"
#include <SDL_vulkan.h>
#include <glm/glm.hpp>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#pragma region STATIC VARIABLES

const float VulkanHelpers::VulkanStaticVariables::s_queuePriority = 1.0f;

std::vector<std::string> VulkanHelpers::VulkanStaticVariables::s_requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#pragma endregion

namespace VulkanHelpers
{

#pragma region INSTANCE CREATION METHODS

	void CreateInstance(const std::string& appName, VkInstance* instance, SDL_Window* window)
	{
		InstanceValidationLayersData layersData;
		layersData.m_layerCount = 0;
		ExtensionLayersData extensionsData;
		extensionsData.m_extensionCount = 0;

#ifdef DEBUG
		if (!GetInstanceValidationLayers(layersData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN INSTANCE CREATION] Validation layers requested, but not available!");
		}
#endif

		if (!GetInstanceExtensionsLayers(extensionsData, window))
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
		appInfo.apiVersion = VK_API_VERSION_1_3;

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
		createinfo.ppEnabledLayerNames = layersData.m_validationLayers.data();
		createinfo.pNext = &debugCreateInfo;
#endif
		createinfo.enabledExtensionCount = (uint32_t)extensionsData.m_extensions.size();
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

	bool CheckInstanceLayers(InstanceValidationLayersData& layersData)
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

	bool CheckInstanceExtensions(ExtensionLayersData& extensionsData, SDL_Window* window)
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

	bool GetInstanceValidationLayers(InstanceValidationLayersData& layersData)
	{
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, nullptr);
		layersData.m_availableLayers.resize(layersData.m_layerCount);
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, layersData.m_availableLayers.data());

		return CheckInstanceLayers(layersData);
	}

	bool GetInstanceExtensionsLayers(ExtensionLayersData& extensionsData, SDL_Window* window)
	{
		return CheckInstanceExtensions(extensionsData, window);
	}

	void CreateSwapChain(SwapChainData& swapChainData, const VulkanHelpers::LogicalDeviceData& deviceData, const VkSurfaceKHR& surface, const glm::ivec2& viewPort)
	{
		PopulateSwapChainData(swapChainData, deviceData.m_physicalDeviceData.m_physicalDevice, surface);
		swapChainData.SetSwapExtent(viewPort);

		uint32_t imageCount = swapChainData.m_capabilities.minImageCount + 1;

		if (swapChainData.m_capabilities.maxImageCount > 0 && imageCount > swapChainData.m_capabilities.maxImageCount) {
			imageCount = swapChainData.m_capabilities.maxImageCount;
		}

		//actual create
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = swapChainData.m_selectedFormat.format;
		createInfo.imageColorSpace = swapChainData.m_selectedFormat.colorSpace;
		createInfo.imageExtent = swapChainData.m_swapExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t graphicsFamilyIndex = deviceData.m_physicalDeviceData.m_graphicsFamilyIndex.value();
		uint32_t presentFamilyIndex = deviceData.m_physicalDeviceData.m_presentFamilyIndex.value();
		uint32_t queueFamilyIndices[] = { graphicsFamilyIndex, presentFamilyIndex };

		if ( graphicsFamilyIndex != presentFamilyIndex) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		createInfo.preTransform = swapChainData.m_capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = swapChainData.m_selectedPresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(deviceData.m_logicalDevice, &createInfo, nullptr, &swapChainData.m_vulkanSwapchain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		swapChainData.GetSwapChainImages(deviceData);
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

#pragma endregion

#pragma region DEVICE CREATION METHODS

	bool GetPhysicalDevices(VkInstance* instance, const VkSurfaceKHR& surface, std::vector<PhysicalDeviceData>& devices)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> vulkanDevices(deviceCount);
		vkEnumeratePhysicalDevices(*instance, &deviceCount, vulkanDevices.data());

		if (deviceCount == 0)
		{
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
			return false;
		}

		for (int i = 0; i < vulkanDevices.size(); i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(vulkanDevices[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU || properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				PhysicalDeviceData deviceData = GetPhysicalDeviceDataWrapper(vulkanDevices[i], surface);
				VkBool32 isValid = 0;
				vkGetPhysicalDeviceSurfaceSupportKHR(deviceData.m_physicalDevice, deviceData.m_graphicsFamilyIndex.value(), surface, &isValid);
				if (isValid)
				{
					devices.push_back(deviceData);
				}
			}
		}

		std::sort(devices.begin(), devices.end(), [](PhysicalDeviceData a, PhysicalDeviceData b)
			{
				return a.score > b.score;
			});

		return true;
	}

	bool PhysicalDeviceData::IsDeviceValidForRender()
	{
		if (m_graphicsFamilyIndex < 0 || m_presentFamilyIndex < 0)
		{
			return false;
		}

		return true;
	}

	PhysicalDeviceData GetPhysicalDeviceDataWrapper(VkPhysicalDevice device, const VkSurfaceKHR& surface)
	{
		PhysicalDeviceData deviceData;
		deviceData.m_physicalDevice = device;
		PopulateDeviceQueues(deviceData, surface);
		GetDeviceScore(deviceData);

		return deviceData;
	}

	void PopulateDeviceQueues(PhysicalDeviceData& device, const VkSurfaceKHR& surface)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device.m_physicalDevice, &queueFamilyCount, nullptr);
		device.m_queues.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device.m_physicalDevice, &queueFamilyCount, device.m_queues.data());

		for (int i = 0; i < device.m_queues.size(); i++)
		{
			if (device.m_queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				device.m_graphicsFamilyIndex = (int)device.m_FamilyIndexes.size();
				device.m_FamilyIndexes.push_back(i);
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device.m_physicalDevice, i, surface, &presentSupport);
			
			if (presentSupport)
			{
				if (i != device.m_graphicsFamilyIndex)
				{
					device.m_presentFamilyIndex = (int)device.m_FamilyIndexes.size();
					device.m_FamilyIndexes.push_back(i);
				}
			}
		}

	}

	void PopulateSwapChainData(SwapChainData& swapChainData, const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainData.m_capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			swapChainData.m_formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainData.m_formats.data());
			swapChainData.SelectFormat();
		}
		else
		{
			throw std::runtime_error("failed to get any surface format modes!");
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			swapChainData.m_presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, swapChainData.m_presentModes.data());
			swapChainData.SelectPresentMode();;
		}
		else
		{
			throw std::runtime_error("failed to get any present modes!");
		}

	}

	void GetDeviceScore(PhysicalDeviceData& deviceData)
	{
		int score = 0;
		vkGetPhysicalDeviceProperties(deviceData.m_physicalDevice, &deviceData.m_properties);
		vkGetPhysicalDeviceFeatures(deviceData.m_physicalDevice, &deviceData.m_features);


		// Discrete GPUs have a significant performance advantage
		if (deviceData.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		// Maximum possible size of textures affects graphics quality
		score += deviceData.m_properties.limits.maxImageDimension2D;

		// Application can't function without geometry shaders
		if (deviceData.m_features.geometryShader) {
			score += 100;
		}

		deviceData.score = score;
	}

	bool GetLogicalDevices(SDL_Window* window, VkInstance* instance, std::vector<PhysicalDeviceData>& physicalDevices, std::vector<LogicalDeviceData>& logicalDevices)
	{
		logicalDevices.resize(physicalDevices.size());
		for (int i = 0; i < physicalDevices.size(); i++)
		{
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			PopulateQueueCreateInfos(queueCreateInfos, physicalDevices[i].m_FamilyIndexes);
			CreateLogicalDevice(window, queueCreateInfos, physicalDevices[i], logicalDevices[i]);
			
			vkGetDeviceQueue(logicalDevices[i].m_logicalDevice, physicalDevices[i].m_FamilyIndexes[physicalDevices[i].m_graphicsFamilyIndex.value()].value(), 0, &logicalDevices[i].m_graphicsQueue);
			vkGetDeviceQueue(logicalDevices[i].m_logicalDevice, physicalDevices[i].m_FamilyIndexes[physicalDevices[i].m_presentFamilyIndex.value()].value(), 0, &logicalDevices[i].m_presentQueue);
		}
		return true;
	}

	void PopulateQueueCreateInfo(VkDeviceQueueCreateInfo& createInfo, uint32_t queueFamilyindex)
	{
		float queuePriority = 1.0f;
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		createInfo.queueFamilyIndex = queueFamilyindex;
		createInfo.queueCount = 1;
		createInfo.pQueuePriorities = &queuePriority;

	}

	void PopulateQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& createInfos, const std::vector <std::optional<uint32_t>>& queueIndices)
	{
		for (int i = 0; i < queueIndices.size(); i++)
		{
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.queueFamilyIndex = queueIndices[i].value();
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &VulkanStaticVariables::s_queuePriority;
			createInfos.push_back(createInfo);
		}
	}

	void CreateLogicalDevice(SDL_Window* window, const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos, PhysicalDeviceData& physicalDevice, LogicalDeviceData& logicalDevice)
	{
		if (!physicalDevice.m_presentFamilyIndex.has_value() && physicalDevice.m_graphicsFamilyIndex.has_value())
		{
			physicalDevice.m_presentFamilyIndex = physicalDevice.m_graphicsFamilyIndex.value();
			logicalDevice.m_unifiedPresentAndGraphics = true;
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &physicalDevice.m_features;

		InstanceValidationLayersData layersData;
		layersData.m_layerCount = 0;
		ExtensionLayersData extensionsData;
		extensionsData.m_extensionCount = 0;

#ifdef DEBUG
		if (!GetInstanceValidationLayers(layersData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN DEVICE CREATION] Validation layers requested, but not available!");
		}
#endif
		GetDeviceExtensions(extensionsData, physicalDevice.m_physicalDevice);

		if (!HasRequiredExtensions(extensionsData))
		{
			throw std::runtime_error("Physical device doesn't have required extensions for rendering!");
		}

#ifdef DEBUG
		createInfo.enabledLayerCount = (uint32_t)layersData.m_validationLayers.size();
		createInfo.ppEnabledLayerNames = layersData.m_validationLayers.data();
#endif
		createInfo.enabledExtensionCount = (uint32_t)extensionsData.m_extensions.size();
		createInfo.ppEnabledExtensionNames = extensionsData.m_extensions.data();
		logicalDevice.m_physicalDeviceData = physicalDevice;
		if (vkCreateDevice(logicalDevice.m_physicalDeviceData.m_physicalDevice, &createInfo, nullptr, &logicalDevice.m_logicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create logical device!");
		}
	}

	void GetDeviceExtensions(ExtensionLayersData& extensionsData, const VkPhysicalDevice& device)
	{

		vkEnumerateDeviceExtensionProperties (device, nullptr, &extensionsData.m_extensionCount, nullptr);
		extensionsData.m_availableExtensions.resize(extensionsData.m_extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsData.m_extensionCount, extensionsData.m_availableExtensions.data());
		
		for (unsigned int i = 0; i < extensionsData.m_extensionCount; i++)
		{
			if (std::strcmp(extensionsData.m_availableExtensions[i].extensionName , "VK_EXT_full_screen_exclusive") == 0 ||
				std::strcmp(extensionsData.m_availableExtensions[i].extensionName , VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME) == 0)
			{
				continue;
			}
			extensionsData.m_extensions.push_back(extensionsData.m_availableExtensions[i].extensionName);
		}
		extensionsData.m_extensions.shrink_to_fit();
		extensionsData.m_extensions;
	}

	bool HasRequiredExtensions(const ExtensionLayersData& extensionsData)
	{
		std::set<std::string> requiredExtensions(VulkanStaticVariables::s_requiredDeviceExtensions.begin(), VulkanStaticVariables::s_requiredDeviceExtensions.end());
		
		for (const auto& extension : extensionsData.m_extensions) {
			requiredExtensions.erase(extension);
		}
		
		return requiredExtensions.empty();
	}

#pragma endregion

#pragma region SURFACE CREATION METHODS

	void CreateVulkanSurface(SDL_Window* window, VulkanData& context)
	{
		SDL_Vulkan_CreateSurface(window, context.m_instance, &context.m_surface);
	}

#pragma endregion

#pragma region CLEAN UP METHODS

	void CleanUp(VulkanData& context)
	{
		for (int i = 0; i < context.m_logicalDevices.size(); i++)
		{
			LogicalDeviceData deviceData = context.m_logicalDevices[i];
			vkDestroyDevice(deviceData.m_logicalDevice, nullptr);
		}

		vkDestroySurfaceKHR(context.m_instance, context.m_surface, nullptr);
		vkDestroyInstance(context.m_instance, nullptr);
		vkDestroySwapchainKHR(context.m_selectedDevice.m_logicalDevice, context.m_swapchain.m_vulkanSwapchain, nullptr);

	}

#pragma endregion


#pragma region HELPER DATA STRUCT FUNCTIONS
	
	bool SwapChainData::IsValid() const
	{
		return !m_formats.empty() && !m_presentModes.empty();
	}

	/*The format member specifies the color channels and types.For example, VK_FORMAT_B8G8R8A8_SRGB means
	that we store the B, G, R and alpha channels in that order with an 8 bit unsigned integer for a
	total of 32 bits per pixel.The colorSpace member indicates if the SRGB color space is supported
	or not using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag.*/
	void SwapChainData::SelectFormat()
	{
		if (m_formats.empty())
		{
			return;
		}

		m_selectedFormat = m_formats[0];
		
		for (unsigned int i = 0; i < m_formats.size(); i ++)
		{
			if (m_formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && m_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				m_selectedFormat = m_formats[i];
				break;
			}
		}
	}

	/*The presentation mode is arguably the most important setting for the swap chain,
	because it represents the actual conditions for showing images to the screen.
	There are four possible modes available in Vulkan :
	
	- VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by your application are transferred
		to the screen right away, which may result in tearing.

	- VK_PRESENT_MODE_FIFO_KHR : The swap chain is a queue where the display takes an
		image from the front of the queue when the display is refreshed and the program inserts 
		rendered images at the back of the queue.If the queue is full then the program has to wait.
		This is most similar to vertical sync as found in modern games.The moment that the display 
		is refreshed is known as "vertical blank".

	- VK_PRESENT_MODE_FIFO_RELAXED_KHR : This mode only differs from the previous one if the 
		application is late and the queue was empty at the last vertical blank.Instead of waiting
		for the next vertical blank, the image is transferred right away when it finally arrives.
		This may result in visible tearing.

	- VK_PRESENT_MODE_MAILBOX_KHR : This is another variation of the second mode.Instead of blocking
		the application when the queue is full, the images that are already queued are simply replaced 
		with the newer ones.This mode can be used to render frames as fast as possible while still avoiding 
		tearing, resulting in fewer latency issues than standard vertical sync.This is commonly known as 
		"triple buffering", although the existence of three buffers alone does not necessarily mean that the 
		framerate is unlocked.

	Only the VK_PRESENT_MODE_FIFO_KHR mode is guaranteed to be available, but for energy efficiency VK_PRESENT_MODE_MAILBOX_KHR works just as well
	we'll again have to write a function that looks for the best mode that is available:*/
	void SwapChainData::SelectPresentMode()
	{
		if (m_presentModes.empty())
		{
			return;
		}

		m_selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;

		for (unsigned int i = 0; i < m_presentModes.size(); i++)
		{
			if (m_presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_selectedPresentMode = m_presentModes[i];
			}
		}
	}

#undef max
	void SwapChainData::SetSwapExtent(const glm::ivec2& extentSize)
	{
		if (m_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			m_swapExtent.width = m_capabilities.currentExtent.width;
			m_swapExtent.height = m_capabilities.currentExtent.height;
		}
		else
		{
			const int minWidth = m_capabilities.minImageExtent.width;
			const int minHeight = m_capabilities.minImageExtent.height;
			const int maxWidth = m_capabilities.maxImageExtent.width;
			const int maxHeight = m_capabilities.maxImageExtent.height;
			m_swapExtent.width = glm::clamp(extentSize.x, minWidth, maxWidth);
			m_swapExtent.height = glm::clamp(extentSize.y, minHeight, maxHeight);
		}
	}

	void SwapChainData::GetSwapChainImages(const VulkanHelpers::LogicalDeviceData& logicalDevice)
	{
		unsigned int imageCount = 0;
		vkGetSwapchainImagesKHR(logicalDevice.m_logicalDevice, m_vulkanSwapchain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(logicalDevice.m_logicalDevice, m_vulkanSwapchain, &imageCount, m_swapChainImages.data());
	}


	void VulkanData::SelectDevice()
	{
		//placeholder code need to make a proper device selection algorithm
		m_selectedDevice = m_logicalDevices[0];

		//go through all devices
		//IsDeviceValidForRender
		//m_selectedDevice = 
	}
#pragma endregion

}
#endif // USE_VULKAN
