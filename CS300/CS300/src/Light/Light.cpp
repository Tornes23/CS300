#include "Light.h"
#include "../Shader/Shader.h"
#include <GLM/gtc/type_ptr.hpp>


Light::Light(LightType type, glm::vec3 position, glm::vec3 direction, Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic)
{
	mPosition = position;
	mDirection = direction;

	mAmbientColor = ambient;
	mDiffuseColor = diffuse;
	mSpecularColor = specular;

	mType = type;

	mConstant = constant;
	mLinear = linear;
	mQuadratic = quadratic;

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
	shader->SetVec3Uniform("lightSource.Position",  mPosition);
	shader->SetVec3Uniform("lightSource.Direction", mDirection);

	shader->SetVec4Uniform("lightSource.Ambient",  mAmbientColor.GetColor());
	shader->SetVec4Uniform("lightSource.Diffuse",  mDiffuseColor.GetColor());
	shader->SetVec4Uniform("lightSource.Specular", mSpecularColor.GetColor());

	shader->SetFloatUniform("lightSource.Constant",  mConstant);
	shader->SetFloatUniform("lightSource.Linear",    mLinear);
	shader->SetFloatUniform("lightSource.Quadratic", mQuadratic);

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
