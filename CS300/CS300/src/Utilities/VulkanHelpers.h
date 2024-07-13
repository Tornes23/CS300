#ifdef USE_VULKAN
#pragma once
#include <string>
#include <vector>
#include <optional>
#include <vulkan/vulkan.h>

struct SDL_Window;

namespace VulkanHelpers
{
#pragma region STATIC VARIABLES

	static float s_queuePriority = 1.0f;

#pragma endregion

#pragma region HELPER STRUCTURES 

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
		VkPhysicalDeviceProperties m_properties;//physical device properties
		VkPhysicalDeviceFeatures m_features;//save physical device features
		std::vector<VkQueueFamilyProperties> m_queues;
		std::vector <std::optional<uint32_t>> m_FamilyIndexes;
		std::optional<uint32_t> m_graphicsFamilyIndex;
		std::optional<uint32_t> m_presentFamilyIndex;//index to the queue to present images to the surface we created
		int score = 0;

		bool IsDeviceValidForRender();
		//bool IsDeviceValidForRtx();//in a future for memory or RTX operation for which I don't care of rendering capability

	};

	struct LogicalDeviceData
	{
		VkDevice m_logicalDevice{};
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
		bool m_unifiedPresentAndGraphics = false;
	};

	struct VulkanData
	{
		VkInstance m_instance;
		VkDevice m_currentDevice;
		VkSurfaceKHR m_surface;
		VkSwapchainKHR m_swapchain;
		std::vector<VulkanHelpers::PhysicalDeviceData> m_physicalDevices;
		std::vector<VulkanHelpers::LogicalDeviceData> m_logicalDevices;
#ifdef DEBUG
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif
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

#pragma endregion

#pragma region INSTANCE CREATION METHODS

	void CreateInstance(const std::string& appName, VkInstance* instance, SDL_Window* window);
#ifdef DEBUG
	VkResult CreateDebugCallback(DebugCallbackData& debugData);
	void DestroyDebugCallback(DebugCallbackData& debugData);
#endif
	bool CheckLayers(ValidationLayersData& layerData);
	bool CheckExtensions(ExtensionLayersData& extensionsData, SDL_Window* window);
	bool GetValidationLayers(ValidationLayersData& layerData);
	bool GetExtensionsLayers(ExtensionLayersData& extensionsData, SDL_Window* window);
	
#pragma endregion

#pragma region SURFACE CREATION METHODS

	void CreateVulkanSurface(SDL_Window* window, VulkanData& context);

#pragma endregion

#pragma region DEVICE CREATION METHODS

	bool GetPhysicalDevices(VkInstance* instance, const VkSurfaceKHR& surface, std::vector<PhysicalDeviceData>& devices);
	PhysicalDeviceData GetPhysicalDeviceDataWrapper(VkPhysicalDevice device, const VkSurfaceKHR& surface);
	void PopulateDeviceQueues(PhysicalDeviceData& device, const VkSurfaceKHR& surface);
	void GetDeviceScore(PhysicalDeviceData& deviceData);

	bool GetLogicalDevices(SDL_Window* window, VkInstance* instance, std::vector<PhysicalDeviceData>& physicalDevices, std::vector<LogicalDeviceData>& logicalDevices);
	void PopulateQueueCreateInfo(VkDeviceQueueCreateInfo& createInfo, uint32_t queueFamilyindex);
	void PopulateQueueCreateInfo(std::vector<VkDeviceQueueCreateInfo>& createInfos, const std::vector <std::optional<uint32_t>>& queuesIndices);
	void CreateLogicalDevice(SDL_Window* window, const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos, PhysicalDeviceData& physicalDevice, LogicalDeviceData& logicalDevice);
	void GetDeviceExtensions(ExtensionLayersData& extensionsData, const VkPhysicalDevice& device);
#pragma endregion


#pragma region CLEAN UP METHODS

	void CleanUp(VulkanData& context);

#pragma endregion



}

#endif // USE_VULKAN
