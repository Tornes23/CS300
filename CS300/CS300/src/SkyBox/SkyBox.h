#pragma once
#include "../Texture/Texture.h"
#include "../Shader/Shader.h"
#include "../Model/Model.h"

class SkyBox
{
public:

	SkyBox(std::string cubePath = "./src/Texture/resources/CubeMap");

	Texture GetTexture() const;
	Model GetModel() const;
	ShaderProgram GetShader() const;

private:

	std::string mCubeMapPath;
	
	ShaderProgram mShader;
	
	Texture mCubeMap;
	
	Model mModel;
};

