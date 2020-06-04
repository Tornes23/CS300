#pragma once
#include "../Texture/Texture.h"

class ShaderProgram;

class Material
{
public:

	Material(Color diffuse, Color specular, float shininess, std::string texture, float ambient = 0.5F);
	Material(std::string texture);

	Texture& GetTexture();

	const Color GetDiffuse() const;
	const Color GetSpecular() const;

	float GetShininess() const;
	float GetAmbient() const;

	void SetUniforms(ShaderProgram* shader);

	Texture mTexture;

	Color mDiffuseColor;
	Color mSpecularColor;

	float mShininess;
	float mAmbientCof;
};