#include "Material.h"
#include "../Shader/Shader.h"
#include <GLM/gtc/type_ptr.hpp>

Material::Material(Color diffuse, Color specular, Color ambient, float shininess, std::string texture) : mTexture(texture)
{

	mDiffuseColor = diffuse;
	mSpecularColor = specular;
	mAmbientColor = ambient;

	mShininess = shininess;
}

Material::Material(std::string texture) : mTexture(texture)
{
	mDiffuseColor = Color::White;
	mSpecularColor = Color::White;
	mAmbientColor = Color::Black;

	mShininess = 50;
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

const Color Material::GetAmbient() const
{
	return mAmbientColor;
}

void Material::SetUniforms(ShaderProgram * shader)
{
	mTexture.SetActiveTexture();

	//CKECK THIS
	shader->SetVec3Uniform("material.AmbientColor",  mAmbientColor.GetColor());
	shader->SetVec3Uniform("material.DiffuseColor",  mDiffuseColor.GetColor());
	shader->SetVec3Uniform("material.SpecularColor", mSpecularColor.GetColor());

	shader->SetFloatUniform("material.Shininess", mShininess);
}
