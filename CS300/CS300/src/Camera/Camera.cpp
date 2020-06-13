/*!**************************************************************************
\file    Camera.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 1

\date    Sun Jun 7 06:44:41 2020

\brief
This file contains the implementation of the camera class

The functions included are:
- Camera::Camera();
- void Camera::Render(Window& target, std::vector<GameObject*>& objects);
- void Camera::Update();
- void Camera::ComputePos();
- void Camera::DrawTriangle(GameObject* target);
- void Camera::DrawNormals(GameObject * target);
- void Camera::DrawLights();
- glm::mat4x4 Camera::CreatePerspective();
- glm::mat4x4 Camera::CreateCameraMat();
- void Camera::AddLight();
- void Camera::RemoveLight();
- void Camera::ChangeLights();
- void Camera::UpdateLights();
- void Camera::Edit();
- void SetAnimation();
- void Camera::ApplyLight(ShaderProgram& shader, glm::mat4x4& w2Cam);
- void Camera::AddShader(const std::string & vertex, const std::string & fragment);
- void Camera::AddAllShaders();
- ShaderProgram Camera::GetShader();
- ShaderProgram Camera::GetNormalsShader();

***************************************************************************/
#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>
#include <IMGUI/imgui.h>
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
	mRadius = 40.0F;

	mWireframe = false;
	mRenderNormals = false;
	mAveragedNormals = false;
	mLightAnimation = true;

	mMode = LightingMap;

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
		
		currentShader.SetVec3Uniform("camPositon", mPosition);

		//setting the texture of the object as active
		objects[i]->mMaterial.SetUniforms(&currentShader);

		if (mMode >= LightingMap && mMode < Regular)
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

	if (mMode >= LightingMap && mMode < Regular)
		DrawLights();

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
	Edit();

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

	bool lastAnimation = mLightAnimation;

	if (KeyTriggered(M))
		mWireframe = !mWireframe;

	if (KeyTriggered(N))
		mRenderNormals = !mRenderNormals;

	if (KeyTriggered(F))
		mAveragedNormals = !mAveragedNormals;

	if (KeyTriggered(T))
	{
		mMode = static_cast<RenderMode>((mMode + 1) % Count);
	}

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

	if (lastAnimation != mLightAnimation)
		SetAnimation();

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

/**************************************************************************
*!
\fn     Camera::DrawNormals

\brief
Renders the normals

\param  GameObject* target
the object to be rendered

*
**************************************************************************/
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

/**************************************************************************
*!
\fn     Camera::DrawLights

\brief
Renders the light object

*
**************************************************************************/
void Camera::DrawLights()
{
	//for each light we have
	for (unsigned i = 0; i < mLights.size(); i++)
	{
		//getting and using the shader
		ShaderProgram lightShader = mLights[i].GetShader();
		lightShader.Use();

		//getting the model to world matrix
		glm::mat4x4 m2w = mLights[i].GetM2W();

		//setting the necessary matrices for the render
		lightShader.SetMatUniform("m2w", glm::value_ptr(m2w));
		lightShader.SetMatUniform("view", glm::value_ptr(mCameraMatrix));
		lightShader.SetMatUniform("projection", glm::value_ptr(mPerspective));

		//rendering
		mLights[i].Render();
	}
}

/**************************************************************************
*!
\fn     Camera::ApplyLight

\brief
Renders the light object

\param  ShaderProgram& shader
the shader program to apply the light on

\param  glm::mat4x4& w2Cam
the world to camera matrix

*
**************************************************************************/
void Camera::ApplyLight(ShaderProgram& shader, glm::mat4x4& w2Cam)
{
	//setting if the averaged normals are used or not
	shader.SetIntUniform("Average", mAveragedNormals ? 1 : 0);

	//setting the number of light sources
	shader.SetIntUniform("lightCount", (int)mLights.size());

	//for each light set the uniforms
	for (int i = 0; i < mLights.size(); i++)
	{
		mLights[i].Setuniforms("lightSources[" + std::to_string(i) + "]", &shader, w2Cam, mPosition);
	}

}

/**************************************************************************
*!
\fn     Camera::ComputePos

\brief
Recomputes the position of the camera

*
**************************************************************************/
void Camera::ComputePos()
{
	//cmoputing the position and setting it
	float posX = (mRadius * cosf(glm::radians(mRotations.y))) * sinf(glm::radians(mRotations.x));
	float posY = (mRadius * sinf(glm::radians(mRotations.y)));
	float posZ = (mRadius * cosf(glm::radians(mRotations.y))) * cosf(glm::radians(mRotations.x));

	mPosition = glm::vec3(posX, posY, posZ);
}

/**************************************************************************
*!
\fn     Camera::AddAllShaders

\brief
Adds all the necessary shader for the camera to work

*
**************************************************************************/
void Camera::AddAllShaders()
{
	//adding the shaders
	AddShader("./src/Shader/programs/NormalMap.vs"         , "./src/Shader/programs/NormalMap.fs"         );
	AddShader("./src/Shader/programs/NormalMapColors.vs"   , "./src/Shader/programs/NormalMapColors.fs"   );
	AddShader("./src/Shader/programs/LightingTexture.vs"   , "./src/Shader/programs/LightingTexture.fs"   );
	AddShader("./src/Shader/programs/LightingColor.vs"     , "./src/Shader/programs/LightingColor.fs"     );
	AddShader("./src/Shader/programs/Texture.vs"           , "./src/Shader/programs/Texture.fs"           );
	AddShader("./src/Shader/programs/Mapping.vs"           , "./src/Shader/programs/Mapping.fs"           );
	AddShader("./src/Shader/programs/NormalColoring.vs"    , "./src/Shader/programs/NormalColoring.fs"    );
	AddShader("./src/Shader/programs/TangentColoring.vs"   , "./src/Shader/programs/TangentColoring.fs"   );
	AddShader("./src/Shader/programs/BitangentColoring.vs" , "./src/Shader/programs/BitangentColoring.fs" );
	AddShader("./src/Shader/programs/Mapping.vs"           , "./src/Shader/programs/Mapping.fs"           );
	AddShader("./src/Shader/programs/Normals.vs"           , "./src/Shader/programs/Normals.fs"        , "./src/Shader/programs/Normals.gs");
	AddShader("./src/Shader/programs/NormalsAverage.vs"    , "./src/Shader/programs/NormalsAverage.fs" , "./src/Shader/programs/Normals.gs");

}

/**************************************************************************
*!
\fn     Camera::AddLight

\brief
Adds a light to the level

*
**************************************************************************/
void Camera::AddLight()
{
	//getting the size
	size_t size = mLights.size();

	//if is 8 (the limit)
	if (size == 8)
		return;

	//clearing the vector
	if (!mLights.empty())
		mLights.clear();

	//computing the angle step
	float angleStep = 360.0F / (size + 1);

	//for the number of light + 1 (the added one)
	for (size_t i = 0; i < size + 1; i++)
	{
		//compute the angle of start
		float angle = i * angleStep;

		//setting the initial position
		glm::vec3 initPos(angle, 0, 0);

		//adding it to the container
		mLights.push_back(Light(mLightMode, initPos));
	}
}

/**************************************************************************
*!
\fn     Camera::RemoveLight

\brief
Removes a light from the level

*
**************************************************************************/
void Camera::RemoveLight()
{
	//if there is only one light return
	if (mLights.size() == 1)
		return;

	//removing the las added light
	if (!mLights.empty())
		mLights.pop_back();
}

/**************************************************************************
*!
\fn     Camera::ChangeLights

\brief
Changes the light to a specific mode

*
**************************************************************************/
void Camera::ChangeLights()
{
	//for each light in the container
	for (unsigned i = 0; i < mLights.size(); i++)
	{
		//setting the type
		mLights[i].SetType(mLightMode);
	}
}

/**************************************************************************
*!
\fn     Camera::UpdateLights

\brief
Upadtes the lights
*
**************************************************************************/
void Camera::UpdateLights()
{
	//updating the lights
	for (int i = 0; i < mLights.size(); i++)
		mLights[i].Update();
}

/**************************************************************************
*!
\fn     Camera::Edit

\brief
Wrapping function for imgui
*
**************************************************************************/
void Camera::Edit()
{
	//creating a window
	if (!ImGui::Begin("Lights"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//for each light
	for (unsigned i = 0; i < mLights.size(); i++)
	{
		//setting the tab title
		std::string title = "Light";

		title += std::to_string(i);

		//calling to the edit of the light
		if (ImGui::CollapsingHeader(title.c_str()))
		{
			mLights[i].Edit(i);
		}
	}

	ImGui::End();
}

/**************************************************************************
*!
\fn     Camera::SetAnimation

\brief
Sets the animation boolean for the lights


*
**************************************************************************/
void Camera::SetAnimation()
{
	for (unsigned i = 0; i < mLights.size(); i++)
	{
		mLights[i].SetAnimation(mLightAnimation);
	}
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
	//returning the shader
	return mShaders[mMode];
	
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
	//if the averaged normals is on
	if (mAveragedNormals)
		return mShaders[11];

	return mShaders[10];//normals shader
}

const Light Camera::GetLight() const
{
	//returning the light
	return mLights[0];
}

