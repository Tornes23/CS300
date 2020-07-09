/*!**************************************************************************
\file    CubeMap.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 4 

\date    Fri Jul 10 00:19:30 2020

\brief	 This file containsm the implementation of the 
CubeMap class

The functions included are:
- CubeMap::CubeMap(std::string cubePath);
- const GLuint CubeMap::GetHandle(bool loaded) const;
- void CubeMap::SetCubeMapActive(bool loaded);
- void CubeMap::GenMipMap();
- void CubeMap::GenFBOCube();
- int CubeMap::GetIndex(bool loaded);

***************************************************************************/
#include <iostream>
#include <vector>
#include <SDL2/SDL_image.h>
#include "CubeMap.h"
#include "../Utilities/Utilities.h"

/**************************************************************************
*!
\fn     CubeMap::CubeMap

\brief 
The constructor for the class

\param  std::string cubePath


*
**************************************************************************/
CubeMap::CubeMap(std::string cubePath)
{
	//setting the member variables
	mCubeMapPath = cubePath;

	//generating the buffers
	GenBuffer();

	//loading the image from the path
	LoadCubeImage();

	//setting the sampling parameters
	SetParameters();

	//generating th FBO cube
	GenFBOCube();

	//setting the texture indexes
	mCubeIndex = 1;

	mGenCubeIndex = 2;
}

/**************************************************************************
*!
\fn     Texture::GenBuffer

\brief
Generates the cube map buffer

*
**************************************************************************/
void CubeMap::GenBuffer()
{
	//generating the buffers
	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
}

/**************************************************************************
*!
\fn     Texture::SetParameter

\brief
Sets the parameters

\param  GLint param1
The sampling parameter to set

\param  GLint param2
The sampling parameter to set

*
**************************************************************************/
void CubeMap::SetParameters(GLint param1, GLint param2)
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, param1);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, param2);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

/**************************************************************************
*!
\fn     CubeMap::GetHandle

\brief 


\param  bool loaded
boolean to know if we want to use a loaded texture or not

\return const GLuint
the handle to the texture

*
**************************************************************************/
const GLuint CubeMap::GetHandle(bool loaded) const
{
	if(!loaded)
		return mFBOCubeHandle;
	
	return mHandle;
}

/**************************************************************************
*!
\fn     Texture::GetFormat

\brief
Gets the format of the file loaded

\param  SDL_Surface * texture
 pointer to the loaded file

*
**************************************************************************/
void CubeMap::GetFormat(SDL_Surface * texture)
{
	//if a loaded image was given check it's format otherwise set to RGBA
	if (texture == nullptr)
		mFormat = GL_RGBA;
	else
		mFormat = texture->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
}

/**************************************************************************
*!
\fn     Texture::FreeSurface

\brief
Frees the loaded image

\param  SDL_Surface* surface
 pointer to the loaded image

*
**************************************************************************/
void CubeMap::FreeSurface(SDL_Surface* surface)
{
	//if the pointer is valid call to free the memory
	if (surface != nullptr)
		SDL_FreeSurface(surface);
}

/**************************************************************************
*!
\fn     CubeMap::SetCubeMapActive

\brief 
Sets the cube map texture as active

\param  bool loaded
if we want to use the loaded one or not

*
**************************************************************************/
void CubeMap::SetCubeMapActive(bool loaded)
{
	if (loaded)
	{
		//setting the texture as active
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
	}
	else
	{
		//setting the texture as active
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mFBOCubeHandle);
	}
}

/**************************************************************************
*!
\fn     CubeMap::GenMipMap

\brief 
Generates mip maps

*
**************************************************************************/
void CubeMap::GenMipMap()
{
	//generating mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
}

/**************************************************************************
*!
\fn     CubeMap::GenFBOCube

\brief 
Generates the FBO cube map to use for the enviroment map

*
**************************************************************************/
void CubeMap::GenFBOCube()
{
	//generating cube map texture
	glGenTextures(1, &mFBOCubeHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mFBOCubeHandle);

	//size variable
	const unsigned EnvMapSize = 512;

	//for each side of the cube generate a 2d image 
	for (GLuint i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, EnvMapSize, EnvMapSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	//setting the sampling parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	// Create and set up the FBO
	glGenFramebuffers(6, mFrameCube);
	for (GLuint i = 0; i < 6; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameCube[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mFBOCubeHandle, 0);
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}

	//binding the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**************************************************************************
*!
\fn     CubeMap::GetIndex

\brief 
Gets the index for the texture

\param  bool loaded
if we want the loaded one or not

\return int
the index

*
**************************************************************************/
int CubeMap::GetIndex(bool loaded)
{
	if (!loaded)
		return mGenCubeIndex;

	return mCubeIndex;
}

/**************************************************************************
*!
\fn     Texture::LoadCubeImage

\brief
Loads the .pngs of the specified path

\param  const std::string & folder
tha path to the source folder

*
**************************************************************************/
void CubeMap::LoadCubeImage()
{
	std::vector<std::string> images;

	Utils::GetFileNames(mCubeMapPath, images);

	for (unsigned i = 0; i < images.size(); i++)
	{
		//calling to the SDL_Image load function
		mTexture = IMG_Load(images[i].c_str());

		//if it was loaded succesfully
		if (mTexture == nullptr)
		{
			std::cout << "Error while trying to load " << images[i] << " texture" << std::endl;
			exit(-1);
		}

		//getting the rgb or rgba
		GetFormat(mTexture);

		//generating a 2d image
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mFormat, mTexture->w, mTexture->h, 0, mFormat, GL_UNSIGNED_BYTE, mTexture->pixels);

		//generating mipmaps
		GenMipMap();

		//freeing the data
		FreeSurface(mTexture);

	}
}


