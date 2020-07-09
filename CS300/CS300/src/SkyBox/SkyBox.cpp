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
#include "SkyBox.h"

/**************************************************************************
*!
\fn     SkyBox::SkyBox

\brief 
The constructor for the class

\param  std::string cubePath
the path where the images that create the skybox are

*
**************************************************************************/
SkyBox::SkyBox(std::string cubePath) : mShader("./src/Shader/programs/SkyBox.vs", "./src/Shader/programs/SkyBox.fs"), mCubeMap(cubePath), mModel()
{

}

/**************************************************************************
*!
\fn     SkyBox::GetCubeMap

\brief 
Getter function for the cube map

\return CubeMap
the cube map

*
**************************************************************************/
CubeMap SkyBox::GetCubeMap() const
{
	return mCubeMap;
}

/**************************************************************************
*!
\fn     SkyBox::GetModel

\brief 
getter function for model

\return Model
the model

*
**************************************************************************/
Model SkyBox::GetModel() const
{
	return mModel;
}

/**************************************************************************
*!
\fn     SkyBox::GetShader

\brief 
Getter for the shader program

\return ShaderProgram
the shader


*
**************************************************************************/
ShaderProgram SkyBox::GetShader() const
{
	return mShader;
}

