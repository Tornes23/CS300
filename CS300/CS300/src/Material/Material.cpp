#include "Material.h"

Material::Material(Color diffuse, Color specular, float shininess, std::string texture, float ambient) : mTexture(texture)
{

	mDiffuseColor = diffuse;
	mSpecularColor = specular;

	mShininess = shininess;
	mAmbientCof = ambient;
}

Material::Material(std::string texture) : mTexture(texture)
{
	mDiffuseColor = Color::White;
	mSpecularColor = Color::White;

	mShininess = 0.0F;
	mAmbientCof = 1.0F;

	mTexture = texture;
}

Texture& Material::GetTexture()
{
	return mTexture;
}

const Color Material::GetDiffuse() const
{
	return mDiffuseColor;
}

const Color Material::GetSpecular() const
{
	return mSpecularColor;
}

const float Material::GetShininess() const
{
	return mShininess;
}

const float Material::GetAmbient() const
{
	return mAmbientCof;
}
