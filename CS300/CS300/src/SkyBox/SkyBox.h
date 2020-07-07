#pragma once
#include "../CubeMap/CubeMap.h"
#include "../Shader/Shader.h"
#include "../Model/Model.h"

class SkyBox
{
public:

	SkyBox(std::string cubePath = "./src/Texture/resources/CubeMap");

	CubeMap GetCubeMap() const;
	Model GetModel() const;
	ShaderProgram GetShader() const;

private:

	ShaderProgram mShader;
	
	CubeMap mCubeMap;
	
	Model mModel;
};

