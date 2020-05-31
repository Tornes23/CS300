#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 direction, Color ambient, Color diffuse, Color specular, LightType type)
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

const Light::LightType Light::GetType() const
{
	return mType;
}
