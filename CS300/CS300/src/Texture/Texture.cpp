/*!**************************************************************************
\file    Texture.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 07:31:38 2020

\brief   This file contains the 
implementation of the Texture class

The functions included are:
- Texture::Texture(const std::string & filename);
- void Texture::SetActiveTexture();
- void Texture::UploadTexture(void* data);
- void Texture::GenBuffers();
- void Texture::GetFormat(SDL_Surface * texture);
- void Texture::GenMipMap();
- void Texture::SetParameter(GLint param);
- const GLuint Texture::GetHandle() const;
- void Texture::LoadImage(const std::string & image);
- void Texture::FreeSurface(SDL_Surface* surface);

***************************************************************************/


#include <iostream>
#include <vector>
#include <SDL2/SDL_image.h>
#include "../Utilities/Utilities.h"
#include "Texture.h"

/**************************************************************************
*!
\fn     Texture::Texture

\brief 
The non default constructor of the class

\param  const std::string & filename
the name of the image to loadf

*
**************************************************************************/
Texture::Texture(std::string filename, bool cubeMap)
{
	//setting the member variables
	mTexture = nullptr;
	mFilename = filename;

	if (!cubeMap)
	{
		//creating a default 6x6 texture in case is needed
		Color defaultTexture[] =
		{
			Color::Blue,   Color::Cyan,   Color::Green,  Color::Yellow, Color::Red,    Color::Pink,
			Color::Cyan,   Color::Green,  Color::Yellow, Color::Red,    Color::Pink,   Color::Blue,
			Color::Green,  Color::Yellow, Color::Red,    Color::Pink,   Color::Blue,   Color::Cyan,
			Color::Yellow, Color::Red,    Color::Pink,   Color::Blue,   Color::Cyan,   Color::Green,
			Color::Red,    Color::Pink,   Color::Blue,   Color::Cyan,   Color::Green,  Color::Yellow,
			Color::Pink,   Color::Blue,   Color::Cyan,   Color::Green,  Color::Yellow, Color::Red
		};

		//if the filename is other than the default one
		if (filename != "./src/Texture/resources/default.png")
			LoadImage(filename);//load it
		else
		{
			//setting the widht and height to 6 
			mWidth = 6;
			mHeight = 6;
		}

		//generating the buffers
		GenBuffers();

		//getting the format of the texture
		GetFormat(mTexture);

		//setting the sampling parameter
		if (filename != "./src/Texture/resources/default.png")
			SetParameter();
		else
			SetParameter(GL_NEAREST, GL_NEAREST);

		//depending if an image was actually loaded or not upload the default texture or te loaded image
		if (mTexture == nullptr)
			UploadTexture((unsigned char*)defaultTexture);
		else
			UploadTexture(mTexture->pixels);

		//generating a mipmap
		GenMipMap();

		//freeing the loaded image
		FreeSurface(mTexture);
	}
	else
	{
		GenCubeBuffer();
	
		LoadCubeImage(mFilename);

		SetCubeParameters();
		
	}
}

/**************************************************************************
*!
\fn     Texture::SetActiveTexture

\brief 
Sets the texture as the active one

*
**************************************************************************/
void Texture::SetActiveTexture()
{
	//setting the texture as active
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mHandle);
}

void Texture::SetCubeMapTexture()
{
	//setting the texture as active
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
}

void Texture::SetNormalMap()
{
	//setting the texture as active
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, mHandle);
}

/**************************************************************************
*!
\fn     Texture::UploadTexture

\brief 
Uploads the texture data to the GPU

\param  void* data
 pointer to the texture data

*
**************************************************************************/
void Texture::UploadTexture(void* data)
{
	// Give pixel data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0, mFormat, GL_UNSIGNED_BYTE, data);
}

/**************************************************************************
*!
\fn     Texture::GenBuffers

\brief 
Generates the buffers

*
**************************************************************************/
void Texture::GenBuffers()
{
	//generating the buffer for the texture
	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_2D, mHandle);
}

/**************************************************************************
*!
\fn     Texture::GenCubeBuffer

\brief
Generates the cube map buffer

*
**************************************************************************/
void Texture::GenCubeBuffer()
{
	glGenTextures(1, &mHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
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
void Texture::GetFormat(SDL_Surface * texture)
{
	//if a loaded image was given check it's format otherwise set to RGBA
	if (texture == nullptr)
		mFormat = GL_RGBA;
	else
		mFormat = texture->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
}

/**************************************************************************
*!
\fn     Texture::GenMipMap

\brief 
Generates a MipMap

*
**************************************************************************/
void Texture::GenMipMap()
{
	//generating mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
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
void Texture::SetParameter(GLint param1, GLint param2)
{
	//setting the parameters for the texture to clamp to the maximum
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//setting the sampling mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param2);

}

void Texture::SetCubeParameters(GLint param1, GLint param2)
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, param1);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, param2);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

/**************************************************************************
*!
\fn     Texture::GetHandle

\brief 
Gets the Handle for the texture

\return const GLuint
The value of the handle

*
**************************************************************************/
const GLuint Texture::GetHandle() const
{
	//returning the handle
	return mHandle;
}

/**************************************************************************
*!
\fn     Texture::LoadImage

\brief 
Loads the .png of the specified path

\param  const std::string & image
tha path to the image

*
**************************************************************************/
void Texture::LoadImage(const std::string & image)
{
	//calling to the SDL_Image load function
	mTexture = IMG_Load(image.c_str());

	//if it was loaded succesfully
	if (mTexture == nullptr)
	{
		std::cout << "Error while trying to load " << image << " texture" << std::endl;
		exit(-1);
	}

	//setting the width and height
	mWidth = mTexture->w;
	mHeight = mTexture->h;
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
void Texture::LoadCubeImage(std::string & folder)
{
	std::vector<std::string> images;

	Utils::GetFileNames(folder, images);

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

/**************************************************************************
*!
\fn     Texture::FreeSurface

\brief 
Frees the loaded image

\param  SDL_Surface* surface
 pointer to the loaded image

*
**************************************************************************/
void Texture::FreeSurface(SDL_Surface* surface)
{
	//if the pointer is valid call to free the memory
	if (surface != nullptr)
		SDL_FreeSurface(surface);
}

int Texture::GetTexIndex() const
{
	if (mFilename == "./src/Texture/resources/default.png")
		return 0;
	else
		return 1;

	return 0;
}

