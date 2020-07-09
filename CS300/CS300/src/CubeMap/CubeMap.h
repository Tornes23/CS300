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
#pragma once
#include <string>
#include "../FrameBuffer/FrameBuffer.h"

struct SDL_Surface;

class CubeMap
{
public:
	CubeMap(std::string cubePath = "./src/Texture/resources/CubeMap");

	void GenBuffer();

	void SetParameters(GLint param1 = GL_LINEAR, GLint param2 = GL_LINEAR);

	const GLuint GetHandle(bool loaded = true) const;

	void LoadCubeImage();

	void GetFormat(SDL_Surface* texture = nullptr);

	void FreeSurface(SDL_Surface* surface);

	void SetCubeMapActive(bool loaded = true);

	void GenMipMap();

	void GenFBOCube();

	int GetIndex(bool loaded = true);

	std::string mCubeMapPath;

	GLuint mHandle;
	GLuint mFBOCubeHandle;
	GLuint mFormat;

	int mCubeIndex;
	int mGenCubeIndex;

	GLuint mFrameCube[6];

	SDL_Surface* mTexture;

};
