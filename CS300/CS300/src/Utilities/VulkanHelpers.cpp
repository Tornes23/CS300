#ifdef USE_VULKAN
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <CustomDebug/VulkanDebug.h>
#include "VulkanHelpers.h"
#include <SDL_vulkan.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define VK_KHR_win32_surface 1
#define VK_KHR_WIN32_SURFACE_SPEC_VERSION 6
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

namespace VulkanHelpers
{

#pragma region INSTANCE CREATION METHODS

	void CreateInstance(const std::string& appName, VkInstance* instance, SDL_Window* window)
	{
		ValidationLayersData layersData;
		layersData.m_layerCount = 0;
		ExtensionLayersData extensionsData;
		extensionsData.m_extensionCount = 0;

#ifdef DEBUG
		if (!GetValidationLayers(layersData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN INSTANCE CREATION] Validation layers requested, but not available!");
		}
#endif

		if (!GetExtensionsLayers(extensionsData, window))
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
		appInfo.apiVersion = VK_API_VERSION_1_0;

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

	bool CheckExtensions(ExtensionLayersData& extensionsData, SDL_Window* window)
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

	bool GetValidationLayers(ValidationLayersData& layersData)
	{
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, nullptr);
		layersData.m_availableLayers.resize(layersData.m_layerCount);
		vkEnumerateInstanceLayerProperties(&layersData.m_layerCount, layersData.m_availableLayers.data());

		return CheckLayers(layersData);
	}

	bool GetExtensionsLayers(ExtensionLayersData& extensionsData, SDL_Window* window)
	{
		return CheckExtensions(extensionsData, window);
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

		devices.resize(deviceCount);

		for (int i = 0; i < vulkanDevices.size(); i++)
		{
			devices[i] = GetPhysicalDeviceDataWrapper(vulkanDevices[i], surface);
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
			
			//if (device.m_graphicsFamilyIndex >= 0 && device.m_presentFamilyIndex >= 0)
			//{
			//	break;
			//}
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
			PopulateQueueCreateInfo(queueCreateInfos, physicalDevices[i].m_FamilyIndexes);
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

	void PopulateQueueCreateInfo(std::vector<VkDeviceQueueCreateInfo>& createInfos, const std::vector <std::optional<uint32_t>>& queueIndices)
	{
		for (int i = 0; i < queueIndices.size(); i++)
		{
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.queueFamilyIndex = queueIndices[i].value();
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &s_queuePriority;
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

		ValidationLayersData layersData;
		layersData.m_layerCount = 0;
		ExtensionLayersData extensionsData;
		extensionsData.m_extensionCount = 0;

#ifdef DEBUG
		if (!GetValidationLayers(layersData))
		{
			//throw exception
			throw std::runtime_error("[VULKAN DEVICE CREATION] Validation layers requested, but not available!");
		}
#endif
		GetDeviceExtensions(extensionsData, physicalDevice.m_physicalDevice);

#ifdef DEBUG
		createInfo.enabledLayerCount = (uint32_t)layersData.m_validationLayers.size();
		createInfo.ppEnabledLayerNames = layersData.m_validationLayers.data();
#endif
		createInfo.enabledExtensionCount = (uint32_t)extensionsData.m_extensions.size();
		createInfo.ppEnabledExtensionNames = extensionsData.m_extensions.data();

		if (vkCreateDevice(physicalDevice.m_physicalDevice, &createInfo, nullptr, &logicalDevice.m_logicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create logical device!");
		}
	}

	void GetDeviceExtensions(ExtensionLayersData& extensionsData, const VkPhysicalDevice& device)
	{

		//vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsData.m_extensionCount, nullptr);
		//extensionsData.m_availableExtensions.resize(extensionsData.m_extensionCount);
		//extensionsData.m_extensions.resize(extensionsData.m_extensionCount);
		//vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsData.m_extensionCount, extensionsData.m_availableExtensions.data());
		//
		//for (int i = 0; i < extensionsData.m_extensionCount; i++)
		//{
		//	extensionsData.m_extensions[i] = extensionsData.m_availableExtensions[i].extensionName;
		//}

		extensionsData.m_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
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

	}

#pragma endregion



}
#endif // USE_VULKAN
