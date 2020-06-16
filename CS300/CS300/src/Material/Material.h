/*!**************************************************************************
\file    Material.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     Assignemnt 1

\date    Sun Jun 07 02:59:33 2020

\brief   This file contains the definition of
the Material class

The functions included are:
- Material::Material(std::string texture, Color ambient, Color diffuse, Color specular, float shininess);
- Texture& Material::GetTexture();
- const Color Material::GetDiffuse() const;
- const Color Material::GetSpecular() const;
- float Material::GetShininess() const;
- const Color Material::GetAmbient() const;
- void Material::SetUniforms(ShaderProgram * shader);

***************************************************************************/

#pragma once
#include "../Texture/Texture.h"

class ShaderProgram;

class Material
{
public:

	Material(std::string texture, std::string normalMap, Color ambient = Color::Black, Color diffuse = Color::White, Color specular = Color::White, float shininess = 15);

	Texture& GetTexture();
	Texture& GetNormalMap();

	const Color GetDiffuse() const;
	const Color GetSpecular() const;
	const Color GetAmbient() const;

	float GetShininess() const;

	void SetUniforms(ShaderProgram* shader);

	Texture mTexture;
	Texture mNormalMap;

	Color mAmbientColor;
	Color mDiffuseColor;
	Color mSpecularColor;

	float mShininess;
};