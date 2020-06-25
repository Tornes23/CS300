/*!**************************************************************************
\file    Light.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     Assignemnt 1

\date    Sun Jun 07 02:42:05 2020

\brief   This file contains the implementation of the light class

The functions included are:
- Light::Light(LightType type, glm::vec3 rotations, glm::vec3 direction, 
				Color ambient, Color diffuse, Color specular, float constant, 
				float linear, float quadratic, float inner, float outer, float falloff);
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
- const bool Light::GetAnimation()
- void Light::SetAnimation(bool toSet)

***************************************************************************/

#pragma once
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include "../Color/Color.h"
#include "../Model/Model.h"
#include "../Shader/Shader.h"

class ShaderProgram;

class Light
{
public:

	enum LightType
	{
		Point,
		Directional,
		Spotlight
	};

	Light(LightType type = LightType::Point, glm::vec3 rotations = glm::vec3(0, 0, 0), glm::vec3 direction = glm::vec3(0, 0, 0),
		 Color ambient = Color::Black, Color diffuse = Color::White, Color specular = Color::White, float constant = 0.0F, float linear = 0.0F,
		 float quadratic = 0.001F, float inner = 30.0F, float outer = 40.0F, float falloff = 3);

	const glm::vec3 GetPosition() const;
	const glm::vec3 GetDirection() const;

	void Setuniforms(std::string shaderString, ShaderProgram* shader, glm::mat4x4& w2Cam, glm::vec3& objPos);
	void Update();
	void Render();
	glm::mat4x4 GetM2W();
	void ComputePos();

	const glm::vec3 GetAmbient();
	const glm::vec3 GetDiffuse();
	const glm::vec3 GetSpecular();
	const bool GetAnimation();
	void SetAnimation(bool toSet);

	LightType GetType() const;
	void  SetType(LightType mode);
	ShaderProgram GetShader() const;

	void Edit(int id);

	void SetShadowMap(GLuint shadow);
	int GetPCFSamples() const;
	void SetPCFSamples(int modify);

private:

	Model mModel;
	ShaderProgram mShader;
	glm::vec3 mScale;
	glm::mat4x4 mModel2World;

	LightType mType;

	glm::vec3 mPosition;
	glm::vec3 mRotations;
	glm::vec3 mDirection;

	Color mAmbientColor;
	Color mDiffuseColor;
	Color mSpecularColor;

	glm::vec3 mAttenuation;

	float mCosInner;
	float mCosOuter;
	float mFallOff;
	float mRadius;

	int mPCFSamples;
	int mBias;

	GLuint mShadowMap;

	bool mbAnimation;
};
