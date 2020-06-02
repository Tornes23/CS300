#include "Light.h"
#include "../Shader/Shader.h"
#include <GLM/gtc/type_ptr.hpp>


Light::Light(LightType type, glm::vec3 position, glm::vec3 direction, Color ambient, Color diffuse, Color specular)
{
	mPosition = position;
	mDirection = direction;

	mAmbientColor = ambient;
	mDiffuseColor = diffuse;
	mSpecularColor = specular;

	mType = type;

}

const glm::vec3 Light::GetPosition() const
{
	return mPosition;
}

const glm::vec3 Light::GetDirection() const
{
	return mDirection;
}

void Light::Setuniforms(ShaderProgram * shader)
{
	shader->SetVec3Uniform("light.Position",  glm::value_ptr(mPosition));
	shader->SetVec3Uniform("light.Direction", glm::value_ptr(mDirection));

	shader->SetVec4Uniform("light.Ambient",  (float*)glm::value_ptr(mAmbientColor.GetColor()));
	shader->SetVec4Uniform("light.Diffuse",  (float*)glm::value_ptr(mDiffuseColor.GetColor()));
	shader->SetVec4Uniform("light.Specular", (float*)glm::value_ptr(mSpecularColor.GetColor()));

}

const Color Light::GetAmbient() const
{
	return mAmbientColor;
}

const Color Light::GetDiffuse() const
{
	return mDiffuseColor;
}

const Color Light::GetSpecular() const
{
	return mSpecularColor;
}

Light::LightType Light::GetType() const
{
	return mType;
}

void Light::Edit()
{
}
