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

#pragma once
#include <string>
#include <GL/glew.h>
#include "../Color/Color.h"

struct SDL_Surface;

class Texture
{
public:

	Texture(const std::string& filename);
	void SetActiveTexture();
	void UploadTexture(void* data);
	void GenBuffers();
	void GetFormat(SDL_Surface* texture = nullptr);
	void GenMipMap();
	void SetParameter(GLint param = GL_NEAREST);
	const GLuint GetHandle() const;
	void LoadImage(const std::string& image);
	void FreeSurface(SDL_Surface* surface);

private:

	std::string mFilename;
	SDL_Surface* mTexture;
	GLuint mHandle;
	GLuint mFormat;
	int mWidth;
	int mHeight;
};