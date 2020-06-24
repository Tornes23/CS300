#include "FrameBuffer.h"

//~
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
}

//~
FrameBuffer::FrameBuffer(int width, int height)
{
	mWidth = width;
	mHeight = height;

	GenRenderBuffer();
	GenDepthBuffer();

	// Revert to the default framebuffer for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//~
void FrameBuffer::GenDepthBuffer()
{
	glGenRenderbuffers(1, &mDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
	//setting it will store depth
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	//attaching
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
}

//~
const GLuint FrameBuffer::GetRenderBuffer() const
{
	return mRenderBuffer;
}

//~
const GLuint FrameBuffer::GetRenderTexture() const
{
	return mRenderTexture;
}

//~
const GLuint FrameBuffer::GetDepthBuffer() const
{
	return mDepthBuffer;
}
//~
void FrameBuffer::UseRenderBuffer()
{
	// Bind created FBO
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer);
}

//~
void FrameBuffer::ClearRenderBuffer()
{
	// Clear framebuffer
	glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
