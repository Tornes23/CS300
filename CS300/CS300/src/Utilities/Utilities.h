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
#include <vector>

class Window;

namespace Utils
{	
	void InitSDL();

	void InitGL(Window* window);

	void ParseShader(std::string& filename, std::string& code);

	void GetFileNames(std::string& path, std::vector<std::string>& container);
}