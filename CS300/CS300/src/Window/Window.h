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
#include <GL/glew.h>
#include <string>

class Window
{
public:
	//constructor
	Window(int width = 1280, int height = 720, const char* title = "CS300");

	//getter function
	glm::ivec2 GetViewport() const;

	//utility functions
	void Update();
	void Clear();
	void DeleteContext();
	void DeleteWindow();
	bool IsClosed() const;
	SDL_Window* GetSDLWindow() const;
	SDL_GLContext GetContext() const;
	void SetQuit(bool close);
	void SwapBuffers();

private:
	//the necessary member variables
	SDL_Window* mWindow;
	SDL_GLContext mContext;
	std::string mTitle;
	glm::ivec2 mSize;
	bool mQuit;
};