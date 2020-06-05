#pragma once
#include "../Texture/Texture.h"

class ShaderProgram;

class Material
{
public:

	Material(Color diffuse, Color specular, Color ambient, float shininess, std::string texture);
	Material(std::string texture);

	Texture& GetTexture();

	const Color GetDiffuse() const;
	const Color GetSpecular() const;
	const Color GetAmbient() const;

	float GetShininess() const;

	void SetUniforms(ShaderProgram* shader);

	Texture mTexture;

	Color mAmbientColor;
	Color mDiffuseColor;
	Color mSpecularColor;

	float mShininess;
};