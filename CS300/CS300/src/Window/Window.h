/*!**************************************************************************
\file    Window.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 07:36:44 2020

\brief   This file contains the
implementation of the Window class

The functions included are:
- Window::Window(int width, int height, const char * title);
- glm::vec2 Window::GetViewport() const;
- void Window::Update();
- void Window::Clear();
- void Window::SetQuit(bool close);
- void Window::DeleteContext();
- void Window::DeleteWindow();
- bool Window::IsClosed() const;
- SDL_Window * Window::GetSDLWindow() const;
- SDL_GLContext Window::GetContext() const;

***************************************************************************/
#pragma once
#include <GLM/vec2.hpp>
#include <SDL2/SDL.h>
#ifdef USE_OPENGL
#include <GL/glew.h>
#endif
#ifdef USE_VULKAN
#include <vulkan/vulkan.h>
#include "Utilities/VulkanHelpers.h"
#endif
#include <string>
#include <vector>
#include <optional>

class Window
{
public:

#ifdef USE_VULKAN
	struct VulkanInstanceData
	{
		VkInstance m_instance;
		VkSwapchainKHR m_swapchain;
		VkDevice m_device;
		std::vector<VulkanHelpers::PhysicalDeviceData> m_physicalDevices;
#ifdef DEBUG
		VkDebugUtilsMessengerEXT m_debugMessenger;
#endif
	};
#endif // USE_VULKAN

	//constructor
	Window(int width = 1280, int height = 720, const char* title = "CS300");

	//getter function
	glm::vec2 GetViewport() const;
	std::string GetTitle() const;

	//utility functions
	void Update();
	void Clear();
	void DeleteContext();
	void DeleteWindow();
	bool IsClosed() const;
	SDL_Window* GetSDLWindow() const;
#ifdef USE_OPENGL
	SDL_GLContext GetContext() const;
#endif // USE_OPENGL
#ifdef USE_VULKAN
	const Window::VulkanInstanceData& GetContext() const;
	Window::VulkanInstanceData& GetMutableContext();
	void SelectVulkanDevice();
#endif // USE_VULKAN
	void SetQuit(bool close);
	void SwapBuffers();

private:
	//the necessary member variables
	SDL_Window* mWindow;
#ifdef USE_OPENGL
	SDL_GLContext mContext;
#endif // USE_OPENGL
#ifdef USE_VULKAN
	VulkanInstanceData m_context;
#endif // USE_VULKAN
	std::string mTitle;
	glm::ivec2 mSize;
	bool mQuit;
};