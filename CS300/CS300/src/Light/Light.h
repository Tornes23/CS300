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
		 Color ambient = Color::Black, Color diffuse = Color::White, Color specular = Color::White, float constant = 0.0F, float linear = 0.0F, float quadratic = 0.001F);

	const glm::vec3 GetPosition() const;
	const glm::vec3 GetDirection() const;

	void Setuniforms(std::string shaderString, ShaderProgram* shader, glm::mat4x4& w2Cam, glm::vec3& objPos);
	void Update();
	void Render();
	glm::mat4x4 GetM2W();

	const glm::vec3 GetAmbient();
	const glm::vec3 GetDiffuse();
	const glm::vec3 GetSpecular();

	LightType GetType() const;
	void  SetType(LightType mode);
	ShaderProgram GetShader() const;

	void Edit(int id);

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
};
