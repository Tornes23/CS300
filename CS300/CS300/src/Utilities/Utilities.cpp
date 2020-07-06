/*!**************************************************************************
\file    Utilities.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 07:39:18 2020

\brief   This file contains the 
implementation of the function in the Utils namespace

The functions included are:
- void Utils::InitSDL();
- void Utils::InitGL(Window* window);
- void Utils::ParseShader(std::string & filename, std::string & code);

***************************************************************************/


#include <iostream>
#include <fstream>
#include <filesystem>
#include <GL/glew.h>
#include "Utilities.h"
#include "../Window/Window.h"

/**************************************************************************
*!
\fn     Utils::InitSDL

\brief 
Initializes the SDL library

*
**************************************************************************/
void Utils::InitSDL()
{
	//initializing the SDL library to video only
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}
}

/**************************************************************************
*!
\fn     Utils::InitGL

\brief 
Initializes OpenGL

\param  Window* window
The window we will use with OpenGl

*
**************************************************************************/
void Utils::InitGL(Window* window)
{
	//initializing the glew library
	if (glewInit() != GLEW_OK)
	{
		SDL_GL_DeleteContext(window->GetContext());
		SDL_DestroyWindow(window->GetSDLWindow());
		std::cout << "GLEW Error: Failed to init" << std::endl;
		SDL_Quit();
		exit(1);
	}
	
	//depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//back face removal
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

/**************************************************************************
*!
\fn     Utils::ParseShader

\brief 
Parses the shader from the given file to a string

\param  std::string & filename
the file to parse

\param  std::string & code
the string which will containt the parsed code

*
**************************************************************************/
void Utils::ParseShader(std::string & filename, std::string & code)
{
	//stream to read from the file
	std::ifstream inFile(filename, std::ios::in);

	//checking if it was opened
	if (!inFile)
	{
		std::cout << "ERROR WHILE TRYING TO OPEN " << filename << " FILE" << std::endl;

	}

	//reading the code from the file and adding it to the string
	while (inFile.good())
	{
		int c = inFile.get();
		if (!inFile.eof())
			code.push_back((char)c);
	}
	//closing the file
	inFile.close();
}

void Utils::GetFileNames(std::string& path, std::vector<std::string>& container)
{
	namespace fileSystem = std::experimental::filesystem;

	std::string filename;

	for (auto it : fileSystem::directory_iterator(path))
	{
		if (it.path().string().rfind("\\") != std::string::npos)
			filename = it.path().string().substr(it.path().string().rfind("\\") + 1);

		container.push_back(path + "/" + filename);
	}
}

