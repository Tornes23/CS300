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

	mAttenuation.x = constant;
	mAttenuation.y = linear;
	mAttenuation.z = quadratic;

}

const glm::vec3 Light::GetPosition() const
{
	return mPosition;
}

const glm::vec3 Light::GetDirection() const
{
	return mDirection;
}

void Light::Setuniforms(ShaderProgram * shader, glm::mat4x4& w2Cam)
{
	shader->SetIntUniform("lightSource.Type", mType);

	shader->SetVec3Uniform("lightSource.Position",  mPosition);
	shader->SetVec3Uniform("lightSource.Direction", mDirection);
	shader->SetVec3Uniform("lightSource.PosInCamSpc", w2Cam * glm::vec4(mPosition, 1.0));

	shader->SetVec3Uniform("lightSource.Ambient",  mAmbientColor.GetColor());
	shader->SetVec3Uniform("lightSource.Diffuse",  mDiffuseColor.GetColor());
	shader->SetVec3Uniform("lightSource.Specular", mSpecularColor.GetColor());

	shader->SetVec3Uniform("lightSource.Attenuation", mAttenuation);

}

const glm::vec3 Light::GetAmbient()
{
	return mAmbientColor.GetColor();
}

const glm::vec3 Light::GetDiffuse()
{
	return mDiffuseColor.GetColor();
}

const glm::vec3 Light::GetSpecular()
{
	return mSpecularColor.GetColor();
}

Light::LightType Light::GetType() const
{
	return mType;
}

void Light::Edit()
{
}
