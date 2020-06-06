/*!**************************************************************************
\file    Camera.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 06:44:41 2020

\brief
This file contains the implementation of the camera class

The functions included are:
- Camera::Camera();
- void Camera::Render(Window& target, std::vector<GameObject*>& objects);
- void Camera::Update();
- void Camera::DrawTriangle(GameObject* target);
- void Camera::DrawNormals(GameObject * target);
- glm::mat4x4 Camera::CreatePerspective();
- glm::mat4x4 Camera::CreateCameraMat();
- void Camera::AddShader(const std::string & vertex, const std::string & fragment);
- ShaderProgram Camera::GetShader();
- ShaderProgram Camera::GetNormalsShader();

***************************************************************************/


#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include "Camera.h"
#include "../GameObject/GameObject.h"
#include "../Input/Input.h"

/**************************************************************************
*!
\fn     Camera::Camera

\brief 
The default Constructor of the class

*
**************************************************************************/
Camera::Camera(glm::vec3 direction)
{
	mView = glm::normalize(direction);

	mUp = glm::vec3(0, 1, 0);

	//initializing all the vues to 0
	ComputePos();
	mNear = 0.1F;
	mFar = 100.0F;
	mRadius = 25.0F;

	mWireframe = false;
	mRenderNormals = false;
	mAveragedNormals = false;
	mTextureMapping = false;
	mLighting = true;

	mLightMode = Light::LightType::Point;

	AddAllShaders();
	AddLight();
}

/**************************************************************************
*!
\fn     Camera::Render

\brief 
The render function

\param  Window& target
The window in which it will render

\param  std::vector<GameObject*>& objects
The game objects to render

*
**************************************************************************/
void Camera::Render(std::vector<GameObject*>& objects)
{
	GLenum error = glGetError();
	//getting the shader which will be used
	
	//for each object
	for (unsigned i = 0; i < objects.size(); i++)
	{
		//if is not active skip it
		if (!(objects[i]->mActive))
			continue;

		ShaderProgram currentShader = GetShader();
		Light lightSource;

		currentShader.Use();

		//Setting the matrix uniforms
		currentShader.SetMatUniform("view", glm::value_ptr(mCameraMatrix));
		currentShader.SetMatUniform("projection", glm::value_ptr(mPerspective));

		//generate the model to world of the object
		glm::mat4x4 m2w_object = objects[i]->GenerateM2W();
		glm::mat4x4 m2w_normal = glm::transpose(glm::inverse(mCameraMatrix * m2w_object));

		//setting the uniform matrix
		currentShader.SetMatUniform("m2w", glm::value_ptr(m2w_object));
		currentShader.SetMatUniform("m2w_normal", glm::value_ptr(m2w_normal));

		//setting the texture of the object as active
		objects[i]->mMaterial.SetUniforms(&currentShader);

		if (mLighting)
			ApplyLight(currentShader, mCameraMatrix);


		//if wireframe is on change the render mode
		if (!mWireframe)//if wireframe is not togled on
			glPolygonMode(GL_FRONT, GL_FILL);
		else
			glPolygonMode(GL_FRONT, GL_LINE);

		//rendering the trianlges
		DrawTriangle(objects[i]);

		//if the normals have to be rendered
		if (mRenderNormals)
		{
			currentShader = GetNormalShader();

			currentShader.Use();

			currentShader.SetMatUniform("m2w", glm::value_ptr(m2w_object));
			currentShader.SetMatUniform("m2w_normal", glm::value_ptr(m2w_normal));
			currentShader.SetMatUniform("view", glm::value_ptr(mCameraMatrix));
			currentShader.SetMatUniform("projection", glm::value_ptr(mPerspective));

			DrawNormals(objects[i]);
		}
	}

	if (mLighting)
	{
		DrawLights();
	}

	//unbinding the VAOs
	glBindVertexArray(0);
	glUseProgram(0);
}

/**************************************************************************
*!
\fn     Camera::Update

\brief 
The update function for the class

*
**************************************************************************/
void Camera::Update()
{	

#pragma region CAMERA MOVEMENT

	if (KeyDown(W))
	{
		if(mRotations.y + 1.0F < 85.0F)
			mRotations.y += 1.0F;
	}

	if (KeyDown(A))
		mRotations.x -= 1.0F;

	if (KeyDown(S))
	{
		if (mRotations.y - 1.0F > -85.0F)
			mRotations.y -= 1.0F;
	}

	if (KeyDown(D))
		mRotations.x += 1.0F;

	if (KeyDown(Q))
	{
		if(mRadius - 1.0F > 5.0F)
			mRadius -= 1.0F;
	}

	if (KeyDown(E))
		mRadius += 1.0F;

#pragma endregion

#pragma region RENDER MODIFICATIONS

	if (KeyTriggered(M))
		mWireframe = !mWireframe;

	if (KeyTriggered(N))
		mRenderNormals = !mRenderNormals;

	if (KeyTriggered(T))
		mTextureMapping = !mTextureMapping;

	if (KeyTriggered(F))
		mAveragedNormals = !mAveragedNormals;

	if (KeyTriggered(P))
		mLighting = !mLighting;

	Light::LightType lastMode = mLightMode;

	if (KeyDown(num7))
		mLightMode = Light::LightType::Point;

	if (KeyDown(num8))
		mLightMode = Light::LightType::Spotlight;

	if (KeyDown(num9))
		mLightMode = Light::LightType::Directional;

	if (KeyTriggered(R))
		AddLight();

	if (KeyTriggered(Y))
		RemoveLight();

#pragma endregion

	if (lastMode != mLightMode)
		ChangeLights();

	//updating the matrices and the vectors
	ComputePos();
	CreatePerspective();
	CreateCameraMat();
	UpdateLights();
}

/**************************************************************************
*!
\fn     Camera::DrawTriangle

\brief 
Renders a triangle or the outline based on the setted flags

\param  GameObject* target
the object to be rendered

*
**************************************************************************/
void Camera::DrawTriangle(GameObject* target)
{
	GLenum error = glGetError();

	//binding the objects VAO
	glBindVertexArray(target->mModel.GetVAO());
	error = glGetError();

	// Drawing
	if (target->mModel.GetIndexed())
		glDrawElements(GL_TRIANGLES, target->mModel.GetDrawElements(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, target->mModel.GetDrawElements());

	error = glGetError();
}

void Camera::DrawNormals(GameObject * target)
{
	GLenum error = glGetError();

	//binding the objects VAO
	glBindVertexArray(target->mModel.GetNormalVAO());
	error = glGetError();

	// Drawing
	if (target->mModel.GetIndexed())
		glDrawElements(GL_TRIANGLES, target->mModel.GetDrawElements(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, target->mModel.GetDrawElements());

	error = glGetError();
}

void Camera::DrawLights()
{
	for (unsigned i = 0; i < mLights.size(); i++)
	{
		ShaderProgram lightShader = mLights[i].GetShader();

		lightShader.Use();

		glm::mat4x4 m2w = mLights[i].GetM2W();

		lightShader.SetMatUniform("m2w", glm::value_ptr(m2w));
		lightShader.SetMatUniform("view", glm::value_ptr(mCameraMatrix));
		lightShader.SetMatUniform("projection", glm::value_ptr(mPerspective));

		mLights[i].Render();
	}
}

void Camera::ApplyLight(ShaderProgram& shader, glm::mat4x4& w2Cam)
{
	shader.SetIntUniform("Average", mAveragedNormals ? 1 : 0);
	shader.SetIntUniform("lightCount", mLights.size());

	for (int i = 0; i < mLights.size(); i++)
	{
		mLights[i].Setuniforms("lightSources[" + std::to_string(i) + "]", &shader, w2Cam, mPosition);
	}

}

void Camera::ComputePos()
{
	float posX = (mRadius * cosf(glm::radians(mRotations.y))) * sinf(glm::radians(mRotations.x));
	float posY = (mRadius * sinf(glm::radians(mRotations.y)));
	float posZ = (mRadius * cosf(glm::radians(mRotations.y))) * cosf(glm::radians(mRotations.x));

	mPosition = glm::vec3(posX, posY, posZ);
}

void Camera::AddAllShaders()
{
	AddShader("./src/Shader/programs/Texture.vs"          , "./src/Shader/programs/Texture.fs"        );
	AddShader("./src/Shader/programs/Mapping.vs"          , "./src/Shader/programs/Mapping.fs"        );
	AddShader("./src/Shader/programs/LightingTexture.vs"  , "./src/Shader/programs/LightingTexture.fs");
	AddShader("./src/Shader/programs/LightingColor.vs"    , "./src/Shader/programs/LightingColor.fs"  );
	AddShader("./src/Shader/programs/Normals.vs"          , "./src/Shader/programs/Normals.fs"        , "./src/Shader/programs/Normals.gs");
	AddShader("./src/Shader/programs/NormalsAverage.vs"   , "./src/Shader/programs/NormalsAverage.fs" , "./src/Shader/programs/Normals.gs");
}

void Camera::AddLight()
{
	size_t size = mLights.size();

	if (!mLights.empty())
		mLights.clear();

	float angleStep = glm::two_pi<float>() / (size + 1);

	for (size_t i = 0; i < size + 1; i++)
	{
		float angle = i * angleStep;

		glm::vec3 initPos(angle, 0, 0);

		mLights.push_back(Light(mLightMode, initPos));
	}
}

void Camera::RemoveLight()
{
	if (!mLights.empty())
		mLights.pop_back();
}

void Camera::ChangeLights()
{
	for (unsigned i = 0; i < mLights.size(); i++)
	{
		mLights[i].SetType(mLightMode);
	}
}

void Camera::UpdateLights()
{
	for (int i = 0; i < mLights.size(); i++)
		mLights[i].Update();
}

/**************************************************************************
*!
\fn     Camera::CreatePerspective

\brief 
Generates a perspective projection matrix

\return glm::mat4x4
returns the matrix

*
**************************************************************************/
glm::mat4x4 Camera::CreatePerspective()
{
	//creating the matrix
	mPerspective = glm::perspective(glm::radians(60.0F), 1280.0F/720.0F, mNear, mFar);

	//returning the matrix
	return mPerspective;

}

/**************************************************************************
*!
\fn     Camera::CreateCameraMat

\brief 
Creates the camera matrix

\return glm::mat4x4
returns the matrix

*
**************************************************************************/
glm::mat4x4 Camera::CreateCameraMat()
{
	//creating the matrix
	mCameraMatrix = glm::lookAt(mPosition, mView, mUp);
	
	//returning the matrix
	return mCameraMatrix;
}

/**************************************************************************
*!
\fn     Camera::AddShader

\brief 
Adds a shader to the container

\param  const std::string & vertex
 a string containing the address of the vertex shader

\param  const std::string & fragment
a string containing the address of the fragment shader

*
**************************************************************************/
void Camera::AddShader(const std::string & vertex, const std::string & fragment, const std::string& geometry)
{
	//adding the shader to the vector
	mShaders.push_back(ShaderProgram(vertex, fragment, geometry));
}

/**************************************************************************
*!
\fn     Camera::GetShader

\brief 
Returns the shader that has to be used based on the flags set by the user

\return ShaderProgram
returns the shader program

*
**************************************************************************/
ShaderProgram Camera::GetShader()
{
	//if texture mapping is on
	if (mTextureMapping)
	{
		if (mLighting)
			return mShaders[3];

		//return the shader that renders colors based on UV coords
		return mShaders[1];
	}
	else
	{
		if (mLighting)
			return mShaders[2];

		//return the shader that renders the texture
		return mShaders[0];
	}

	//by default return the texture shader
	return mShaders[2];
}

/**************************************************************************
*!
\fn     Camera::GetNormalsShader

\brief 
Returns the shader that has to be used based on the flags set by the user


\return ShaderProgram
returns the shader program


*
**************************************************************************/
ShaderProgram Camera::GetNormalShader()
{
	if (mAveragedNormals)
		return mShaders[5];

	return mShaders[4];//normals shader
}

const Light Camera::GetLight(Light::LightType mode) const
{
	switch (mode)
	{
	case Light::Point:
		return mLights[0];
		break;
	case Light::Directional:
		return mLights[1];
		break;
	case Light::Spotlight:
		return mLights[2];
		break;
	default:
		break;
	}

	return mLights[0];
}

