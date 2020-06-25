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


#include <iostream>
#include "Window.h"
#include "../Input/Input.h"

/**************************************************************************
*!
\fn     Window::Window

\brief 
The default constructor for the class

\param  int width
the width of the window

\param  int height
the height of the window

\param  const char * title
The title of the window

*
**************************************************************************/
Window::Window(int width, int height, const char * title)
{
	//setting the viewport vector
	mSize.x = width;
	mSize.y = height;
	mQuit = false;

	//creating the window
	mWindow = SDL_CreateWindow("CS300", 100, 100, width, height, SDL_WINDOW_OPENGL);

	//if is invalid
	if (mWindow == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	//setting the attributes for the opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//creating the context
	mContext = SDL_GL_CreateContext(mWindow);

	//if is invalid
	if (mContext == nullptr)
	{
		SDL_DestroyWindow(mWindow);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
}

/**************************************************************************
*!
\fn     Window::GetViewport

\brief 
Getter function for the viewport

\return glm::vec2
Returns a vec2 containing the viewport size

*
**************************************************************************/
glm::ivec2 Window::GetViewport() const
{
	//rerturning the viewpor size
	return mSize;
}

/**************************************************************************
*!
\fn     Window::Update

\brief 
The update function for the class

*
**************************************************************************/
void Window::Update()
{
	//if the key esc is pressed close
	if (KeyTriggered(Key::Esc))
		mQuit = true;
}

/**************************************************************************
*!
\fn     Window::Clear

\brief 
Clears the window

*
**************************************************************************/
void Window::Clear()
{
	//clearing the window to black and the depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**************************************************************************
*!
\fn     Window::SetQuit

\brief 
Setter function to close the window

\param  bool close
boolean to set

*
**************************************************************************/
void Window::SetQuit(bool close)
{
	//setting the close variable
	mQuit = close;
}

void Window::SwapBuffers()
{
	//swapping the front and back buffers
	SDL_GL_SwapWindow(mWindow);
}

/**************************************************************************
*!
\fn     Window::DeleteContext

\brief 
Deletes the OpenGL context used

*
**************************************************************************/
void Window::DeleteContext()
{
	//freeing the openGL context
	SDL_GL_DeleteContext(mContext);
}

/**************************************************************************
*!
\fn     Window::DeleteWindow

\brief 
Deletes the SDL window

*
**************************************************************************/
void Window::DeleteWindow()
{
	//freeing the sdl window
	SDL_DestroyWindow(mWindow);
}

/**************************************************************************
*!
\fn     Window::IsClosed

\brief 
Function to check if the window is closed

\return bool
 boolean indicating if is closed or not

*
**************************************************************************/
bool Window::IsClosed() const
{
	//returning if is closed
	return mQuit;
}

/**************************************************************************
*!
\fn     Window::GetSDLWindow

\brief 
Returns the SDL window

\return SDL_Window *
 pointer to the SDL window

*
**************************************************************************/
SDL_Window * Window::GetSDLWindow() const
{
	//returning the sdl window
	return mWindow;
}

/**************************************************************************
*!
\fn     Window::GetContext

\brief 
Gets the OpenGL context

\return SDL_GLContext
Returns the context

*
**************************************************************************/
SDL_GLContext Window::GetContext() const
{
	//returning the opengl context
	return mContext;
}


