/*!**************************************************************************
\file    Camera.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 06:44:41 2020

\brief
This file contains the definition of the camera class

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

#pragma once
#include <vector>
#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include <GLM/vec2.hpp>
#include "../Window/Window.h"
#include "../Shader/Shader.h"
#include "../Light/Light.h"

struct GameObject;

class Camera
{
public:

	Camera(glm::vec3 direction = glm::vec3(0,0,-1));
	void Render(std::vector<GameObject*>& objects);
	void Update();
	void DrawTriangle(GameObject* target);
	void DrawNormals(GameObject* target);
	void DrawLights();
	void ApplyLight(ShaderProgram& shader, glm::mat4x4& w2Cam);
	void ComputeVectors();
	void AddAllShaders();
	void AddAllLights();

	glm::mat4x4 CreatePerspective();
	glm::mat4x4 CreateCameraMat();

	void AddShader(const std::string& vertex, const std::string& fragment, const std::string& geometry = "");
	ShaderProgram GetShader();
	ShaderProgram GetNormalShader();

	const Light GetLight(Light::LightType mode) const;

private:

	glm::vec3 mPosition;//camera position
	glm::vec3 mView;//view vector
	glm::vec3 mUp;//up vector
	glm::vec3 mRightVector;//right vector

	//booleans to know how to render
	bool mWireframe;
	bool mRenderNormals;
	bool mTextureMapping;
	bool mAveragedNormals;
	bool mLighting;

	//the needed matrices
	glm::mat4x4 mPerspective;
	glm::mat4x4 mCameraMatrix;

	//data for the camera and the transformations
	float mNear;
	float mFar;
	float mRadius;

	glm::vec3 mRotations;

	Light::LightType mLightMode;

	std::vector<ShaderProgram> mShaders;
	std::vector<Light> mLights;
};