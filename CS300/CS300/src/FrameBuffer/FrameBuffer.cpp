/*!**************************************************************************
\file    FrameBuffer.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 3 

\date    Wed Jul 01 11:02:02 2020

\brief
This file contains the implementation of the FrameBuffer class

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


#include "FrameBuffer.h"

/**************************************************************************
*!
\fn     FrameBuffer::FrameBuffer

\brief 
The default constructor of the class

\param  int width
the width for the frame buffer

\param  int height
the height for the frame buffer

*
**************************************************************************/
FrameBuffer::FrameBuffer(int width, int height)
{
	//setting the variables
	mWidth = width;
	mHeight = height;

	mShadowMapWidth = 1024;
	mShadowMapHeight = 1024;

	mContrast = 1.0F;

	GenRenderBuffer();
	GenDepthBuffer();
}

/**************************************************************************
*!
\fn     FrameBuffer::GenRenderBuffer

\brief 
Generates the Render buffer

*
**************************************************************************/
void FrameBuffer::GenRenderBuffer()
{
	// Create to render texture (use window resolution)
	glGenTextures(1, &mRenderTexture);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Expansion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create and set up the FBO
	glGenFramebuffers(1, &mRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	/* Write Your Code */

	// Set output color texture at attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture, 0);
	GLenum attachements[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachements);

	// Add depth render buffer to the frame buffer
	GLuint depthRenderBuffer;
	//generating
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	//setting it will store depth
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	//attaching
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/**************************************************************************
*!
\fn     FrameBuffer::GenDepthBuffer

\brief 
Generates the Depth buffer

*
**************************************************************************/
void FrameBuffer::GenDepthBuffer()
{
	// Create to render texture (use shadowmap resolution)
	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowMapWidth, mShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Expansion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create and set up the FBO
	glGenFramebuffers(1, &mDepthBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthBuffer);

	// Set output color texture at attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

	GLenum attachements[] = { GL_NONE };

	glDrawBuffers(1, attachements);

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**************************************************************************
*!
\fn     FrameBuffer::GetRenderBuffer

\brief 
Getter function for the Render Buffer

\return const GLuint
The handle for the buffer

*
**************************************************************************/
const GLuint FrameBuffer::GetRenderBuffer() const
{
	return mRenderBuffer;
}

/**************************************************************************
*!
\fn     FrameBuffer::GetRenderTexture

\brief 
Getter function for the Render texture

\return const GLuint
The handle for the texture

*
**************************************************************************/
const GLuint FrameBuffer::GetRenderTexture() const
{
	return mRenderTexture;
}

/**************************************************************************
*!
\fn     FrameBuffer::GetShadowMap

\brief 
Getter function for the ShadowMap texture


\return const GLuint
The handle for the texture

*
**************************************************************************/
const GLuint FrameBuffer::GetShadowMap() const
{
	return mShadowMap;
}

/**************************************************************************
*!
\fn     FrameBuffer::GetContrast

\brief
Getter function for the contrast value


\return const float
The value

*
**************************************************************************/
const float FrameBuffer::GetContrast() const
{
	return mContrast;
}

/**************************************************************************
*!
\fn     FrameBuffer::SetContrast

\brief
Setter function for the copnstrast value


\param float val
value to set

*
**************************************************************************/
void FrameBuffer::SetContrast(float val)
{
	mContrast = val;
}


/**************************************************************************
*!
\fn     FrameBuffer::GetDepthBuffer

\brief 
Getter function for the depth texture


\return const GLuint
The handle for the buffer


*
**************************************************************************/
const GLuint FrameBuffer::GetDepthBuffer() const
{
	return mDepthBuffer;
}
/**************************************************************************
*!
\fn     FrameBuffer::UseRenderBuffer

\brief 
Sets everything to render on the render buffer

*
**************************************************************************/
void FrameBuffer::UseRenderBuffer()
{
	//setting the viewport size
	glViewport(0, 0, mWidth, mHeight);

	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);

	//clearing the buffer
	ClearRenderBuffer();

	//back face removal
	glCullFace(GL_BACK);
}

/**************************************************************************
*!
\fn     FrameBuffer::UseDepthBuffer

\brief 
Sets everything to render on the depth buffer

*
**************************************************************************/
void FrameBuffer::UseDepthBuffer()
{
	//setting the size of the viewport
	glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);

	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthBuffer);
	
	//clearing the buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//back face removal
	glCullFace(GL_FRONT);
}

/**************************************************************************
*!
\fn     FrameBuffer::ClearRenderBuffer

\brief 
clears the Render Buffer

*
**************************************************************************/
void FrameBuffer::ClearRenderBuffer()
{
	// Clear framebuffer
	glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

