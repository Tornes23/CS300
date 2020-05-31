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
Camera::Camera()
{
	//setting the view vector as the 0,0,-1
	mView = glm::vec3(0, 0, -1);

	mUp = glm::vec3(0, 1, 0);

	mRightVector = glm::cross(mUp, mView);
	glm::normalize(mRightVector);

	//initializing all the vues to 0
	mPosition = glm::vec3(0, 0, 0);
	mNear = 0.1F;
	mFar = 100.0F;

	mWireframe = false;
	mRenderNormals = false;
	mAveragedNormals = false;
	mTextureMapping = false;
	mLighting = false;

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
void Camera::Render(Window& target, std::vector<GameObject*>& objects)
{
	GLenum error = glGetError();
	//getting the shader which will be used
	ShaderProgram currentShader = GetShader();

	// Bind the shader program and this object's VAO
	glUseProgram(currentShader.GetHandle());
	error = glGetError();

	//Setting the matrix uniforms
	currentShader.SetUniform("view", glm::value_ptr(mCameraMatrix));
	currentShader.SetUniform("projection", glm::value_ptr(mPerspective));

	//for each object
	for (unsigned i = 0; i < objects.size(); i++)
	{
		//if is not active skip it
		if (!(objects[i]->mActive))
			continue;

		//generate the model to world of the object
		glm::mat4x4 m2w_object = objects[i]->GenerateM2W();

		//setting the uniform matrix
		currentShader.SetUniform("m2w", glm::value_ptr(m2w_object));

		//setting the texture of the object as active
		objects[i]->mMaterial.GetTexture().SetActiveTexture();

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
			// Bind the glsl program and this object's VAO
			glUseProgram(currentShader.GetHandle());
			error = glGetError();
			
			currentShader.SetUniform("m2w", glm::value_ptr(m2w_object));
			currentShader.SetUniform("view", glm::value_ptr(mCameraMatrix));
			currentShader.SetUniform("projection", glm::value_ptr(mPerspective));

			DrawNormals(objects[i]);
		}
	}

	//unbinding the VAOs
	glBindVertexArray(0);
	glUseProgram(0);

	//swapping the front and back buffers
	SDL_GL_SwapWindow(target.GetSDLWindow());
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

#pragma endregion
	//updating the matrices and the fustrum
	CreatePerspective();
	CreateCameraMat();
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

/**************************************************************************
*!
\fn     Camera::DrawNormals

\brief
Renders a triangle or the outline based on the setted flags

\param  GameObject* target
the object to be rendered

*
**************************************************************************/
void Camera::DrawNormals(GameObject* target)
{
	GLenum error = glGetError();

	//binding the objects VAO
	glBindVertexArray(target->mModel.GetNormalVAO(mAveragedNormals));
	error = glGetError();

	// Drawing
	glDrawArrays(GL_LINES, 0, target->mModel.GetNormalCount(mAveragedNormals));

	error = glGetError();
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
void Camera::AddShader(const std::string & vertex, const std::string & fragment)
{
	//adding the shader to the vector
	mShaders.push_back(ShaderProgram(vertex, fragment));
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
		//return the shader that renders colors based on UV coords
		return mShaders[1];
	}
	else
	{
		//return the shader that renders the texture
		return mShaders[0];
	}

	//by default return the texture shader
	return mShaders[0];
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
	{
		return mShaders[3];//return normals & averaged
	}

	return mShaders[2];//normals shader
}





