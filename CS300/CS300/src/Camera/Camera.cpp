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
- void Camera::RenderDepth(Window& target, std::vector<GameObject*>& objects);
- void Camera::Display();
- void Camera::DisplayShadowMap();
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
#include <glm/gtx/transform2.hpp>
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
Camera::Camera(glm::vec3 direction, glm::ivec2 viewport) : mFrameBuffer(viewport.x, viewport.y), mRenderPlane(Model::Shape::Plane), mShadowPlane(Model::Shape::Plane), mSkyBox(), mCotton("./src/Texture/resources/CottonCandy")
{
	mView = glm::normalize(direction);

	mUp = glm::vec3(0, 1, 0);

	//initializing all the vues to 0
	ComputePos();
	mNear = 10.0F;
	mFar = 500.0F;
	mRadius = 50.0F;

	mFOV = 60.0F;

	mViewport = viewport;

	mWireframe = false;
	mRenderNormals = false;
	mAveragedNormals = true;
	mLightAnimation = true;
	mFancyShit = false;

	mMode = Regular;

	mLightMode = Light::LightType::Spotlight;

	AddAllShaders();
}

/**************************************************************************
*!
\fn     Camera::Render

\brief 
The render function

\param  std::vector<GameObject*>& objects
The game objects to render

*
**************************************************************************/
void Camera::Render(std::vector<GameObject*>& objects)
{
	//calling that the render buffer wil be used 
	mFrameBuffer.UseRenderBuffer();

	//getting the shader which will be used
	GLenum error = glGetError();

	RenderSkyBox(mCameraMatrix, mPerspective);

	//for each object
	for (unsigned i = 0; i < objects.size(); i++)
	{
		//if is not active skip it
		if (!(objects[i]->mActive))
			continue;
	
		ShaderProgram currentShader = GetShader();
	
		//calling to use this render
		currentShader.Use();
	
		//setting the uniforms
		if(objects[i]->mName.find("Side") != objects[i]->mName.npos)
			currentShader.SetIntUniform("Mode", 0);
		else
			currentShader.SetIntUniform("Mode", mMode);

		currentShader.SetIntUniform("Averaged", mAveragedNormals ? 1 : 0);

		//Setting the matrix uniforms
		currentShader.SetMatUniform("view", glm::value_ptr(mCameraMatrix));
		currentShader.SetMatUniform("projection", glm::value_ptr(mPerspective));
	
		//generate the model to world of the object
		glm::mat4x4 m2w_object = objects[i]->GenerateM2W();
		glm::mat4x4 m2w_normal = glm::transpose(glm::inverse(m2w_object));
	
		//setting the uniform matrix
		currentShader.SetMatUniform("m2w", glm::value_ptr(m2w_object));

		currentShader.SetVec3Uniform("CamPos", mPosition);

		//setting the texture of the object as active
		objects[i]->mMaterial.SetUniforms(&currentShader);
	
		//if wireframe is on change the render mode
		if (!mWireframe)//if wireframe is not togled on
			glPolygonMode(GL_FRONT, GL_FILL);
		else
			glPolygonMode(GL_FRONT, GL_LINE);
	
		//rendering the trianlges
		DrawTriangle(&(objects[i]->mModel));
	
		//if the normals have to be rendered
		if (mRenderNormals)
		{
			currentShader = GetNormalShader();
	
			currentShader.Use();
	
			currentShader.SetMatUniform("m2w", glm::value_ptr(m2w_object));
			currentShader.SetMatUniform("m2w_normal", glm::value_ptr(m2w_normal));
			currentShader.SetMatUniform("view", glm::value_ptr(mCameraMatrix));
			currentShader.SetMatUniform("projection", glm::value_ptr(mPerspective));
			currentShader.SetIntUniform("Average", mAveragedNormals ? 1 : 0);
	
			DrawNormals(&(objects[i]->mModel));
		}
	}

	//calling to be rendered on to the scren
	Display();
	
	//unbinding the VAOs
	glBindVertexArray(0);
	glUseProgram(0);

}

/**************************************************************************
*!
\fn     Camera::RenderDepth

\brief
The render function for the skybox

*
**************************************************************************/
void Camera::RenderSkyBox(glm::mat4& view, glm::mat4& proj)
{
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	ShaderProgram skybox = mSkyBox.GetShader();

	skybox.Use();

	glm::mat4 camera = glm::mat4(glm::mat3(view));

	skybox.SetMatUniform("view", glm::value_ptr(camera));
	skybox.SetMatUniform("projection", glm::value_ptr(proj));
	skybox.SetIntUniform("cubeMap", mSkyBox.GetCubeMap().GetIndex());

	if (!mFancyShit)
	{
		mSkyBox.GetCubeMap().SetCubeMapActive();

		DrawTriangle(&(mSkyBox.GetModel()));
	}
	else
	{
		mCotton.GetCubeMap().SetCubeMapActive();

		DrawTriangle(&(mCotton.GetModel()));
	}
	glCullFace(GL_BACK);

	glDepthFunc(GL_LESS);
}

/**************************************************************************
*!
\fn     Camera::Display

\brief
The render to screen function 

*
**************************************************************************/
void Camera::Display()
{
	// Bind screen buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Clear screen framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Select shader program, set uniforms and draw 
	ShaderProgram secondPass = GetDisplayShader();
	
	secondPass.Use();
	
	glm::mat4 transform = glm::scale(glm::vec3(2.0F));
	
	secondPass.SetMatUniform("M", glm::value_ptr(transform));
	
	GLenum error = glGetError();
	
	//binding the objects VAO
	glBindVertexArray(mRenderPlane.GetVAO());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFrameBuffer.GetRenderTexture());
	secondPass.SetIntUniform("textureData", 0);
	
	// Draw
	glDrawArrays(GL_TRIANGLES, 0, mRenderPlane.GetDrawElements());


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

	bool lastAnimation = mLightAnimation;

	if (KeyTriggered(M))
		mWireframe = !mWireframe;

	if (KeyTriggered(N))
		mRenderNormals = !mRenderNormals;

	if (KeyTriggered(F))
		mAveragedNormals = !mAveragedNormals;

	if (KeyTriggered(T))
		mMode = static_cast<RenderMode>((mMode + 1) % Count);

	if (KeyTriggered(P))
		mFancyShit = !mFancyShit;

	if (KeyTriggered(Z))
	{
		if (mLights[0].GetPCFSamples() - 1 > 0)
			mLights[0].SetPCFSamples(-1);
	}

	if (KeyTriggered(X))
		mLights[0].SetPCFSamples(1);

	Light::LightType lastMode = mLightMode;

	if (KeyDown(num7))
		mLightMode = Light::LightType::Point;

	if (KeyDown(num8))
		mLightMode = Light::LightType::Spotlight;

	if (KeyDown(num9))
		mLightMode = Light::LightType::Directional;

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

void Camera::GenerateEnviroment(std::vector<GameObject*>& reflectiveObjects, std::vector<GameObject*>& allObjects)
{
	for (unsigned i = 0; i < reflectiveObjects.size(); i++)
	{
		if (!(reflectiveObjects[i]->mActive))
			continue;

		RenderEnviroment(reflectiveObjects[i], allObjects);
	}
}

void Camera::RenderEnviroment(GameObject * target, std::vector<GameObject*>& objects)
{
	glViewport(0, 0, 512, 512);

	CubeMap& targetMap = target->mMaterial.mEnviromentMap;

	std::vector<glm::vec3> viewVecs {glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)};
	std::vector<glm::vec3> upVecs { glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), glm::vec3(0, -1, 0) };

	for (int i = 0; i < 6; i++)
	{
		// Bind created FBO
		glBindFramebuffer(GL_FRAMEBUFFER, targetMap.mFrameCube[i]);

		glClearColor(0.0F, 0.0F, 0.0F, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//create view/ projection matrix and render onto the texture
		glm::mat4x4 view = glm::lookAt(target->mPosition, viewVecs[i], upVecs[i]);

		glm::mat4x4 proj = glm::perspective(glm::radians(90.0F), 1.0F, 0.1F, mFar);

		RenderSkyBox(view, proj);

		//for each object
		for (unsigned j = 0; j < objects.size(); j++)
		{
			//if is not active skip it
			if (!(objects[j]->mActive))
				continue;

			ShaderProgram currentShader = GetShader();

			//calling to use this render
			currentShader.Use();

			//setting the uniforms
			if (objects[j]->mName.find("Side") != objects[j]->mName.npos)
				currentShader.SetIntUniform("Mode", 0);
			else
				currentShader.SetIntUniform("Mode", mMode);

			currentShader.SetIntUniform("Averaged", mAveragedNormals ? 1 : 0);

			//Setting the matrix uniforms
			currentShader.SetMatUniform("view", glm::value_ptr(view));
			currentShader.SetMatUniform("projection", glm::value_ptr(proj));

			//generate the model to world of the object
			glm::mat4x4 m2w_object = objects[j]->GenerateM2W();

			//setting the uniform matrix
			currentShader.SetMatUniform("m2w", glm::value_ptr(m2w_object));

			currentShader.SetVec3Uniform("CamPos", mPosition);

			//setting the texture of the object as active
			objects[j]->mMaterial.SetUniforms(&currentShader);

			//if wireframe is on change the render mode
			if (!mWireframe)//if wireframe is not togled on
				glPolygonMode(GL_FRONT, GL_FILL);
			else
				glPolygonMode(GL_FRONT, GL_LINE);

			//rendering the trianlges
			DrawTriangle(&(objects[j]->mModel));
		}

		//unbinding the VAOs
		glBindVertexArray(0);
		glUseProgram(0);

	}

	glViewport(0, 0, mViewport.x, mViewport.y);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
void Camera::DrawTriangle(Model* target)
{
	GLenum error = glGetError();

	//binding the objects VAO
	glBindVertexArray(target->GetVAO());
	error = glGetError();

	// Drawing
	if (target->GetIndexed())
		glDrawElements(GL_TRIANGLES, target->GetDrawElements(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, target->GetDrawElements());

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
void Camera::DrawNormals(Model * target)
{
	GLenum error = glGetError();

	//binding the objects VAO
	glBindVertexArray(target->GetNormalVAO());
	error = glGetError();

	// Drawing
	if (target->GetIndexed())
		glDrawElements(GL_TRIANGLES, target->GetDrawElements(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, target->GetDrawElements());

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
	AddShader("./src/Shader/programs/Reflection.vs" , "./src/Shader/programs/Reflection.fs" );
	AddShader("./src/Shader/programs/Normals.vs"    , "./src/Shader/programs/Normals.fs"    , "./src/Shader/programs/Normals.gs");
	AddShader("./src/Shader/programs/Quad.vs"       , "./src/Shader/programs/Quad.fs"       );

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
	mPerspective = glm::perspective(glm::radians(mFOV), static_cast<float>(mViewport.x) / static_cast<float>(mViewport.y), mNear, mFar);

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
	return mShaders[1];//normals shader
}

ShaderProgram Camera::GetDisplayShader()
{
	return mShaders[2];
}


