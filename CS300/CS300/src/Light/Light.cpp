#include "Light.h"
#include "../Shader/Shader.h"
#include <GLM/gtc/type_ptr.hpp>
#include "../Input/Input.h"


Light::Light(LightType type, glm::vec3 position, glm::vec3 direction, Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic) 
			: mModel(Model::Shape::Sphere), mShader("./src/Shader/programs/Light.vs", "./src/Shader/programs/Light.fs")
{
	mRotations = glm::vec3(0, 0, 0);
	mScale = glm::vec3(0.5, 0.5, 0.5);
	mRadius = 15.0F;

	float posX = (mRadius * cosf(glm::radians(mRotations.y))) * sinf(glm::radians(mRotations.x));
	float posY = (mRadius * sinf(glm::radians(mRotations.y)));
	float posZ = (mRadius * cosf(glm::radians(mRotations.y))) * cosf(glm::radians(mRotations.x));

	mPosition = glm::vec3(posX, posY, posZ);

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

void Light::Update()
{

	if (KeyDown(I))
		mRotations.y += 1.0F;

	if (KeyDown(J))
		mRotations.x -= 1.0F;

	if (KeyDown(K))
		mRotations.y -= 1.0F;

	if (KeyDown(L))
		mRotations.x += 1.0F;

	if (KeyDown(U))
		mRadius -= 1.0F;

	if (KeyDown(O))
		mRadius += 1.0F;

	GetM2W();
}

void Light::Render()
{
	GLenum error = glGetError();

	//binding the objects VAO
	glBindVertexArray(mModel.GetVAO());
	error = glGetError();

	// Drawing
	if (mModel.GetIndexed())
		glDrawElements(GL_TRIANGLES, mModel.GetDrawElements(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, mModel.GetDrawElements());

	error = glGetError();
}

glm::mat4x4 Light::GetM2W()
{
	glm::mat4x4 identity(1.0);

	float posX = (mRadius * cosf(glm::radians(mRotations.y))) * sinf(glm::radians(mRotations.x));
	float posY = (mRadius * sinf(glm::radians(mRotations.y)));
	float posZ = (mRadius * cosf(glm::radians(mRotations.y))) * cosf(glm::radians(mRotations.x));

	mPosition = glm::vec3(posX, posY, posZ);

	mModel2World = glm::translate(identity, mPosition);
	mModel2World = glm::scale(mModel2World, mScale);

	return mModel2World;
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

ShaderProgram Light::GetShader() const
{
	return mShader;
}

void Light::Edit()
{
}
