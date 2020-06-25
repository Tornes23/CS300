/*!**************************************************************************
\file    Camera.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 1

\date    Sun Jun 7 06:44:41 2020

\brief
This file contains the definition of the camera class

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

#pragma once
#include <vector>
#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include <GLM/vec2.hpp>
#include "../Window/Window.h"
#include "../Shader/Shader.h"
#include "../Light/Light.h"
#include "../FrameBuffer/FrameBuffer.h"

struct GameObject;

class Camera
{
public:

	typedef enum RenderMode
	{
		Shadows,
		NormalColoring,
		TangentColoring,
		BitangentColoring,
		Count

	} RenderMode;

	Camera(glm::vec3 direction = glm::vec3(0,0,-1), glm::ivec2 viewport = glm::ivec2(1280, 720));
	void Render(std::vector<GameObject*>& objects);
	void RenderDepth(std::vector<GameObject*>& objects);
	void Display();
	void ComputePos();
	void Update();

	void DrawTriangle(GameObject* target);
	void DrawNormals(GameObject* target);
	void DrawLights();

	void AddAllShaders();

	void ApplyLight(ShaderProgram& shader, glm::mat4x4& w2Cam);
	void AddLight();
	void RemoveLight();
	void ChangeLights();
	void UpdateLights();
	void Edit();
	void SetAnimation();

	glm::mat4x4 CreatePerspective();
	glm::mat4x4 CreateCameraMat();

	void AddShader(const std::string& vertex, const std::string& fragment, const std::string& geometry = "");
	ShaderProgram GetShader();
	ShaderProgram GetNormalShader();
	ShaderProgram GetDisplayShader();
	ShaderProgram GetDepthShader();

	const Light GetLight() const;

private:

	glm::vec3 mPosition;//camera position
	glm::vec3 mView;//view vector
	glm::vec3 mUp;//up vector
	glm::vec3 mRightVector;//right vector

	//booleans to know how to render
	bool mWireframe;
	bool mRenderNormals;
	bool mAveragedNormals;
	bool mLightAnimation;

	RenderMode mMode;

	//the needed matrices
	glm::mat4x4 mPerspective;
	glm::mat4x4 mCameraMatrix;

	//data for the camera and the transformations
	float mNear;
	float mFar;
	float mRadius;
	glm::ivec2 mViewport;

	glm::vec3 mRotations;

	Light::LightType mLightMode;

	FrameBuffer mFrameBuffer;

	std::vector<ShaderProgram> mShaders;
	std::vector<Light> mLights;

	Model mRenderPlane;

	int mPCFSamples;
};