/*!**************************************************************************
\file    Light.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     Assignemnt 1 

\date    Sun Jun 07 02:42:05 2020

\brief   This file contains the implementation of the light class

The functions included are:
- Light::Light(LightType type, glm::vec3 rotations, glm::vec3 direction, Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, float inner, float outer);
- const glm::vec3 Light::GetPosition() const;
- const glm::vec3 Light::GetDirection() const;
- void Light::Setuniforms(std::string shaderString, ShaderProgram * shader, glm::mat4x4& w2Cam, glm::vec3& camPos);
- void Light::Update();
- void Light::Render();
- glm::mat4x4 Light::GetM2W();
- const glm::vec3 Light::GetAmbient();
- const glm::vec3 Light::GetDiffuse();
- const glm::vec3 Light::GetSpecular();
- Light::LightType Light::GetType() const;
- void Light::SetType(LightType mode);
- ShaderProgram Light::GetShader() const;
- void Light::Edit(int id);

***************************************************************************/


#include <GLM/gtc/type_ptr.hpp>
#include <IMGUI/imgui.h>
#include "Light.h"
#include "../Shader/Shader.h"
#include "../Input/Input.h"

/**************************************************************************
*!
\fn     Light::Light

\brief 
The constructor of the class

\param  LightType type
the light mode

\param  glm::vec3 rotations
 vec3 containing the rotations on x, y, and z

\param  glm::vec3 direction
the direction in which is looking at

\param  Color ambient
the ambient color

\param  Color diffuse
the diffuse color

\param  Color specular
the specular color

\param  float constant
the constant attenuation value

\param  float linear
the linear attenuation value

\param  float quadratic
the quadratic attenuation value

\param  float inner
the Cosine inner attenuation value

\param  float outer
the cosine of the outer angle for the attenuation

*
**************************************************************************/
Light::Light(LightType type, glm::vec3 rotations, glm::vec3 direction, Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, float inner, float outer)
			: mModel(Model::Shape::Sphere), mShader("./src/Shader/programs/Light.vs", "./src/Shader/programs/Light.fs")
{
	//setting the rotations
	mRotations = rotations;
	mScale = glm::vec3(0.5, 0.5, 0.5);
	mRadius = 15.0F;

	//computing the position
	float posX = (mRadius * cosf(glm::radians(mRotations.y))) * sinf(glm::radians(mRotations.x));
	float posY = (mRadius * sinf(glm::radians(mRotations.y)));
	float posZ = (mRadius * cosf(glm::radians(mRotations.y))) * cosf(glm::radians(mRotations.x));

	mPosition = glm::vec3(posX, posY, posZ);

	//setting the direction
	mDirection = direction;

	//setting the color
	mAmbientColor = ambient;
	mDiffuseColor = diffuse;
	mSpecularColor = specular;

	//setting the color
	mType = type;

	//setting the attenuation variables
	mAttenuation.x = constant;
	mAttenuation.y = linear;
	mAttenuation.z = quadratic;

	mCosInner = inner;
	mCosOuter = outer;

}

/**************************************************************************
*!
\fn     Light::GetPosition

\brief 
Gets the positionofn the light

\return const glm::vec3
the position of the light

*
**************************************************************************/
const glm::vec3 Light::GetPosition() const
{
	//returning the position
	return mPosition;
}

/**************************************************************************
*!
\fn     Light::GetDirection

\brief 
Gets the direction of the light

\return const glm::vec3
the direction vector

*
**************************************************************************/
const glm::vec3 Light::GetDirection() const
{
	//returning the direction
	return mDirection;
}

/**************************************************************************
*!
\fn     Light::Setuniforms

\brief 
Sets the uniforms variables for the shader

\param  std::string shaderString
the string containing the name of the uniform

\param  ShaderProgram * shader
the shader in which to set the variables

\param  glm::mat4x4& w2Cam
the world to camera matrix

\param  glm::vec3& camPos
the camera position

*
**************************************************************************/
void Light::Setuniforms(std::string shaderString, ShaderProgram * shader, glm::mat4x4& w2Cam, glm::vec3& camPos)
{
	//setting the uniform variables
	shader->SetIntUniform(shaderString + ".Type", mType);

	shader->SetVec3Uniform(shaderString + ".Position",  mPosition);
	shader->SetVec3Uniform(shaderString + ".Direction", w2Cam * glm::vec4(mPosition, 1.0));
	shader->SetVec3Uniform(shaderString + ".PosInCamSpc", w2Cam * glm::vec4(mPosition, 1.0));

	shader->SetVec3Uniform(shaderString + ".AmbientColor", mAmbientColor.GetColor());
	shader->SetVec3Uniform(shaderString + ".DiffuseColor", mDiffuseColor.GetColor());
	shader->SetVec3Uniform(shaderString + ".SpecularColor", mSpecularColor.GetColor());

	shader->SetFloatUniform(shaderString + ".CosInner", mCosInner);
	shader->SetFloatUniform(shaderString + ".CosOuter", mCosOuter);
	
	shader->SetVec3Uniform(shaderString + ".Attenuation", mAttenuation);

}

/**************************************************************************
*!
\fn     Light::Update

\brief 
Updates the light

*
**************************************************************************/
void Light::Update()
{
#pragma region MOVEMENT
	if (KeyDown(I))
		mRotations.y += 1.0F;

	if (KeyDown(J))
		mRotations.x -= 1.0F;

	if (KeyDown(K))
		mRotations.y -= 1.0F;

	if (KeyDown(L))
		mRotations.x += 1.0F;

	if (KeyDown(U))
		mRadius -= 0.5F;

	if (KeyDown(O))
		mRadius += 0.5F;

#pragma endregion

	//computing the position
	ComputePos();

	//computing the model to world matrix
	GetM2W();
}

/**************************************************************************
*!
\fn     Light::Render

\brief 
Renders the light

*
**************************************************************************/
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

/**************************************************************************
*!
\fn     Light::GetM2W

\brief 
Gets the model 2 world matrix

\return glm::mat4x4
the matrix

*
**************************************************************************/
glm::mat4x4 Light::GetM2W()
{
	//creating the matrix
	glm::mat4x4 identity(1.0);

	mModel2World = glm::translate(identity, mPosition);
	mModel2World = glm::scale(mModel2World, mScale);

	//returning the matrix
	return mModel2World;
}

/**************************************************************************
*!
\fn     Light::ComputePos

\brief
Computes the position of the light

\return void

*
**************************************************************************/
void Light::ComputePos()
{
	//computing the position and setting it
	float posX = (mRadius * cosf(glm::radians(mRotations.y))) * sinf(glm::radians(mRotations.x));
	float posY = (mRadius * sinf(glm::radians(mRotations.y)));
	float posZ = (mRadius * cosf(glm::radians(mRotations.y))) * cosf(glm::radians(mRotations.x));

	mPosition = glm::vec3(posX, posY, posZ);
}

/**************************************************************************
*!
\fn     Light::GetAmbient

\brief 
Returns the ambient color

\return const glm::vec3
the color

*
**************************************************************************/
const glm::vec3 Light::GetAmbient()
{
	//returning the color
	return mAmbientColor.GetColor();
}

/**************************************************************************
*!
\fn     Light::GetDiffuse

\brief 
Returns the diffuse color

\return const glm::vec3
the color

*
**************************************************************************/
const glm::vec3 Light::GetDiffuse()
{
	//returning the color
	return mDiffuseColor.GetColor();
}

/**************************************************************************
*!
\fn     Light::GetSpecular

\brief 
Returns the specular color

\return const glm::vec3
the color

*
**************************************************************************/
const glm::vec3 Light::GetSpecular()
{
	//returning the color
	return mSpecularColor.GetColor();
}

/**************************************************************************
*!
\fn     Light::GetType

\brief 
Returns the type of the light

\return Light::LightType
the light type

*
**************************************************************************/
Light::LightType Light::GetType() const
{
	//returning the type
	return mType;
}

/**************************************************************************
*!
\fn     Light::SetType

\brief 
sets the light mode

\param  LightType mode
the mode to set

*
**************************************************************************/
void Light::SetType(LightType mode)
{
	//setting the type
	mType = mode;
}

/**************************************************************************
*!
\fn     Light::GetShader

\brief 
Gets the shader to render the light objects

\return ShaderProgram
the shader program

*
**************************************************************************/
ShaderProgram Light::GetShader() const
{
	//returning the shader
	return mShader;
}

/**************************************************************************
*!
\fn     Light::Edit

\brief 
The function wrapping around Imgui

\param  int id
the id to push

*
**************************************************************************/
void Light::Edit(int id)
{
	//pushing the id
	ImGui::PushID(id);

	//tree to modify the transform of the light
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::DragFloat3("Position", glm::value_ptr(mRotations));
		ImGui::DragFloat3("Scale", glm::value_ptr(mScale));

		ImGui::TreePop();
	}

	//tree to modify the light properties
	if (ImGui::TreeNode("Light Properties"))
	{
		//code to change the light mode
		int shape = mType;

		const char* model[3] = { "Point", "Directional", "Spotlight" };

		if (ImGui::Combo("Light Mode", &shape, model, 3, 4)) 
		{
			switch (shape)
			{
			case 0:
				mType = Point;
				break;
			case 1:
				mType = Directional;

				break;
			case 2:
				mType = Spotlight;
				break;
			}
		}

		//getting the colors and storing them in temporals
		glm::vec4 diffuse = mDiffuseColor.GetColor();
		glm::vec4 specular = mSpecularColor.GetColor();
		glm::vec4 ambient = mAmbientColor.GetColor();

		//imgui interface to edit the color
		ImGui::ColorEdit4("Ambient Color", glm::value_ptr(ambient));
		ImGui::ColorEdit4("Diffuse Color", glm::value_ptr(diffuse));
		ImGui::ColorEdit4("Specular Color", glm::value_ptr(specular));

		//to mnodify the attenuation factors
		ImGui::DragFloat3("Attenuation", glm::value_ptr(mAttenuation));

		ImGui::DragFloat("Inner Cosine", &mCosInner);
		ImGui::DragFloat("Outer Cosine", &mCosOuter);
		ImGui::DragFloat("Fall Off", &mFallOff);
		ImGui::DragFloat("Radius", &mRadius);

		//setting the colors
		mAmbientColor.SetColor(ambient);
		mDiffuseColor.SetColor(diffuse);
		mSpecularColor.SetColor(specular);

		ImGui::TreePop();
	}

	//popping the id
	ImGui::PopID();

}

