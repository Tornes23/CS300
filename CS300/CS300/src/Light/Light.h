#pragma once
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include "../Color/Color.h"

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

	Light(LightType type = LightType::Point, glm::vec3 position = glm::vec3(10, 0, 12.5), glm::vec3 direction = glm::vec3(0, 0, 0),
		 Color ambient = Color::Black, Color diffuse = Color::White, Color specular = Color::White, float constant = 0.5F, float linear = 0.25F, float quadratic = 0.125);

	const glm::vec3 GetPosition() const;
	const glm::vec3 GetDirection() const;

	void Setuniforms(ShaderProgram* shader, glm::mat4x4& w2Cam);

	const glm::vec3 GetAmbient();
	const glm::vec3 GetDiffuse();
	const glm::vec3 GetSpecular();

LightType GetType() const;

	void Edit();

private:
	LightType mType;

	glm::vec3 mPosition;
	glm::vec3 mDirection;

	Color mAmbientColor;
	Color mDiffuseColor;
	Color mSpecularColor;

	glm::vec3 mAttenuation;

	float mCosInner;
	float mCosOuter;
	float mFallOff;
};
