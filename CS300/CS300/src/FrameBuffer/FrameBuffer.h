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

	void UseRenderBuffer();

	void ClearRenderBuffer();


private:

	int mWidth;
	int mHeight;

	GLuint mRenderTexture;
	GLuint mRenderBuffer;
	GLuint mDepthBuffer;

};