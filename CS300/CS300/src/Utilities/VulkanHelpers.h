#ifdef USE_VULKAN
#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

struct SDL_Window;

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

	struct PhysicalDeviceData
	{
		VkPhysicalDevice m_physicalDevice;
		std::vector<VkQueueFamilyProperties> m_queues;
		std::optional<uint32_t> m_graphicsFamilyIndex;
		int score = 0;

	};

#ifdef DEBUG
	struct DebugCallbackData
	{
		VkInstance* m_instance;
		const VkAllocationCallbacks* m_pAllocator;
		VkDebugUtilsMessengerEXT* m_pDebugMessenger;
		VkDebugUtilsMessengerCreateInfoEXT m_createInfo{};

		VkDebugUtilsMessengerCreateInfoEXT GetCreateInfo() const;
		void PopulateCreateinfo();
		void Initialize();
		void Initialize(VkInstance* instance,
						const VkAllocationCallbacks* pAllocator,
						VkDebugUtilsMessengerEXT* pDebugMessenger);
	};
#endif

	void CreateInstance(const std::string& appName, VkInstance* instance, SDL_Window* window);
	void DestroyInstance(VkInstance* instance);
#ifdef DEBUG
	VkResult CreateDebugCallback(DebugCallbackData& debugData);
	void DestroyDebugCallback(DebugCallbackData& debugData);
#endif
	bool CheckLayers(ValidationLayersData& layerData);
	bool CheckExtensions(ExtensionLayersData& extensionsData, SDL_Window* window);
	bool GetValidationLayers(ValidationLayersData& layerData);
	bool GetExtensionsLayers(ExtensionLayersData& extensionsData, SDL_Window* window);
	
	bool GetPhysicalDevices(VkInstance* instance, std::vector<PhysicalDeviceData>& devices);
	PhysicalDeviceData GetPhysicalDeviceDataWrapper(VkPhysicalDevice device);
	void PopulateDeviceQueues(PhysicalDeviceData& device);
	int GetDeviceScore(const VkPhysicalDevice& device) const;



}

#endif // USE_VULKAN
