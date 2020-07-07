#pragma once

#include "../Texture/Texture.h"

class CubeMap
{
public:
	CubeMap(std::string cubePath = "./src/Texture/resources/CubeMap");

	void GenBuffer();

	void SetParameters(GLint param1 = GL_LINEAR, GLint param2 = GL_LINEAR);

	const GLuint GetHandle() const;

	void LoadCubeImage();

	void GetFormat(SDL_Surface* texture = nullptr);

	void FreeSurface(SDL_Surface* surface);

	void SetCubeMapActive();

	void GenMipMap();

private:

	std::string mCubeMapPath;

	GLuint mHandle;
	GLuint mFormat;

	SDL_Surface* mTexture;

};
