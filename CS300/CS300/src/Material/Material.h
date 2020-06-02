#pragma once
#include "../Texture/Texture.h"

class Material
{
public:

	Material(Color diffuse, Color specular, float shininess, std::string texture, float ambient = 1.0F);
	Material(std::string texture);

	Texture& GetTexture();

	const Color GetDiffuse() const;
	const Color GetSpecular() const;

	float GetShininess() const;
	float GetAmbient() const;

	Texture mTexture;

	Color mDiffuseColor;
	Color mSpecularColor;

	float mShininess;
	float mAmbientCof;
};