/*!**************************************************************************
\file    Material.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     Assignemnt 1 

\date    Sun Jun 07 02:59:33 2020

\brief   This file contains the implementation of 
the Material class

The functions included are:
- Material::Material(std::string texture, std::string normalMap, Color ambient, Color diffuse, Color specular, float shininess);
- Texture& Material::GetTexture();
- const Color Material::GetDiffuse() const;
- const Color Material::GetSpecular() const;
- float Material::GetShininess() const;
- const Color Material::GetAmbient() const;
- void Material::SetUniforms(ShaderProgram * shader);

***************************************************************************/


#include "Material.h"
#include "../Shader/Shader.h"
#include <GLM/gtc/type_ptr.hpp>

/**************************************************************************
*!
\fn     Material::Material

\brief 
The constructor of the class

\param  std::string texture
 string containing the path of the texture to use

\param  std::string normalMap
 string containing the path of the normalMap to use

\param  Color ambient
the ambient color

\param  Color diffuse
the diffuse color

\param  Color specular
the specular color

\param  float shininess
the shininess of the material

*
**************************************************************************/
Material::Material(std::string texture, std::string normalMap, Color ambient, Color diffuse, Color specular, float shininess, float refraction) : mTexture(texture), mNormalMap(normalMap)
{
	//setting the variables
	mAmbientColor = ambient;
	mDiffuseColor = diffuse;
	mSpecularColor = specular;

	mShininess = shininess;
	mRefraction = refraction;
}

/**************************************************************************
*!
\fn     Material::GetTexture

\brief 
Getter function for the texture

\return Texture&
the texture

*
**************************************************************************/
Texture& Material::GetTexture()
{
	//returning the texture
	return mTexture;
}

/**************************************************************************
*!
\fn     Material::GetNormalMap

\brief
Getter function for the normal map

\return Texture&
the normal map

*
**************************************************************************/
Texture & Material::GetNormalMap()
{
	//returning the texture
	return mNormalMap;
}

/**************************************************************************
*!
\fn     Material::GetDiffuse

\brief 
Getter function for the diffuse color

\return const Color
the color

*
**************************************************************************/
const Color Material::GetDiffuse() const
{
	//returning the color
	return mDiffuseColor;
}

/**************************************************************************
*!
\fn     Material::GetSpecular

\brief 
Getter function for the specular color

\return const Color
the color

*
**************************************************************************/
const Color Material::GetSpecular() const
{
	//returning the color
	return mSpecularColor;
}

/**************************************************************************
*!
\fn     Material::GetShininess

\brief 
Getter function for the shininess value

\return float
the shininess value

*
**************************************************************************/
float Material::GetShininess() const
{
	//returning the texture
	return mShininess;
}

/**************************************************************************
*!
\fn     Material::GetAmbient

\brief 
Getter function for the ambient color

\return const Color
the color

*
**************************************************************************/
const Color Material::GetAmbient() const
{
	//returning the color
	return mAmbientColor;
}

/**************************************************************************
*!
\fn     Material::SetUniforms

\brief 
Sets the uniform variables

\param  ShaderProgram * shader
 pointer to the shader in which set the variables

*
**************************************************************************/
void Material::SetUniforms(ShaderProgram * shader)
{
	//setting the  texture as the active one
	mTexture.SetActiveTexture();
	shader->SetIntUniform("textureData", mTexture.GetTexIndex());


	mEnviromentMap.SetCubeMapActive(false);
	shader->SetIntUniform("EnviromentMap", mEnviromentMap.GetIndex(false));

	shader->SetFloatUniform("RefractionVal", mRefraction);

}

void Material::SetenviromentMap(CubeMap & enviroment)
{
	mEnviromentMap = enviroment;
}

