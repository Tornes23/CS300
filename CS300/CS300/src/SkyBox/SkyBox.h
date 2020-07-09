/*!**************************************************************************
\file    SkyBox.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 4

\date    Fri Jul 10 00:18:18 2020

\brief	 This file containsm the implementation of the
SkyBox class

The functions included are:
- SkyBox::SkyBox(std::string cubePath);
- CubeMap SkyBox::GetCubeMap() const;
- Model SkyBox::GetModel() const;
- ShaderProgram SkyBox::GetShader() const;

***************************************************************************/
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

