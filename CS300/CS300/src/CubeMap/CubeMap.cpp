#include <iostream>
#include <vector>
#include <SDL2/SDL_image.h>
#include "CubeMap.h"
#include "../Utilities/Utilities.h"

CubeMap::CubeMap(std::string cubePath)
{
	mCubeMapPath = cubePath;

	GenBuffer();

	LoadCubeImage();

	SetParameters();

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
	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
}

/**************************************************************************
*!
\fn     Texture::SetParameter

\brief
Sets the parameters

\param  GLint param
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

void CubeMap::GenMipMap()
{
	//generating mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
}

void CubeMap::GenFBOCube()
{
	glGenTextures(1, &mFBOCubeHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mFBOCubeHandle);

	const unsigned EnvMapSize = 512;

	for (GLuint i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, EnvMapSize, EnvMapSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

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

		GetFormat(mTexture);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mFormat, mTexture->w, mTexture->h, 0, mFormat, GL_UNSIGNED_BYTE, mTexture->pixels);

		GenMipMap();

		FreeSurface(mTexture);

	}
}

