/*!**************************************************************************
\file    Utilities.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 07:39:18 2020

\brief   This file contains the
definition of the function in the Utils namespace

The functions included are:
- void Utils::InitSDL();
- void Utils::InitGL(Window* window);
- void Utils::ParseShader(std::string & filename, std::string & code);

***************************************************************************/
#pragma once
#include <string>

class Window;

namespace Utils
{	
	void InitSDL();
#ifdef USE_OPENGL
	void InitGL(Window* window);
#endif
#ifdef USE_VULKAN
	void InitVulkan(Window* window);
#endif

	void ParseShader(std::string& filename, std::string& code);
}