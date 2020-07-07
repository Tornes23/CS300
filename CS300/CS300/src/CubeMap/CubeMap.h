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
