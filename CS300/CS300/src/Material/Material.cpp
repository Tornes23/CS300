#include "Material.h"
#include "../Shader/Shader.h"
#include <GLM/gtc/type_ptr.hpp>

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

float Material::GetShininess() const
{
	return mShininess;
}

float Material::GetAmbient() const
{
	return mAmbientCof;
}

void Material::SetUniforms(ShaderProgram * shader)
{
	mTexture.SetActiveTexture();

	shader->SetVec4Uniform("material.DiffuseColor",  (float*)glm::value_ptr(mDiffuseColor.GetColor()));
	shader->SetVec4Uniform("material.SpecularColor", (float*)glm::value_ptr(mSpecularColor.GetColor()));

	shader->SetFloatUniform("material.Shininess", mShininess);
	shader->SetFloatUniform("material.AmbientCefficient", mAmbientCof);
}
