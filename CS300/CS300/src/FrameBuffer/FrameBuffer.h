/*!**************************************************************************
\file    FrameBuffer.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 3

\date    Wed Jul 01 11:02:02 2020

\brief
This file contains the definition of the FrameBuffer class

The functions included are:
- FrameBuffer::FrameBuffer(int width, int height);
- void FrameBuffer::GenRenderBuffer();
- void FrameBuffer::GenDepthBuffer();
- const GLuint FrameBuffer::GetRenderBuffer() const;
- const GLuint FrameBuffer::GetRenderTexture() const;
- const GLuint FrameBuffer::GetShadowMap() const;
- const GLuint FrameBuffer::GetDepthBuffer() const;
- void FrameBuffer::UseRenderBuffer();
- void FrameBuffer::UseDepthBuffer();
- void FrameBuffer::ClearRenderBuffer();

***************************************************************************/

#pragma once
#include <GL/glew.h>

class FrameBuffer
{
public:

	FrameBuffer(int width = 1280, int height = 720);
	void GenRenderBuffer();
	void GenDepthBuffer();

	const GLuint GetRenderBuffer() const;
	const GLuint GetRenderTexture() const;
	const GLuint GetDepthBuffer() const;
	const GLuint GetShadowMap() const;
	const float  GetContrast() const;
	
	void  SetContrast(float val);

	void UseRenderBuffer();
	void UseDepthBuffer();

	void ClearRenderBuffer();


private:

	int mWidth;
	int mHeight;

	int mShadowMapWidth;
	int mShadowMapHeight;

	float mContrast;

	GLuint mShadowMap;

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
	GLuint mDepthBuffer;

};