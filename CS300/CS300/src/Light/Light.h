#pragma once
#include <GLM/vec3.hpp>
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
		 Color ambient = Color::White, Color diffuse = Color::White, Color specular = Color::White, float constant = 0.5F, float linear = 0.25F, float quadratic = 0.125);

	const glm::vec3 GetPosition() const;
	const glm::vec3 GetDirection() const;

	void Setuniforms(ShaderProgram* shader);

	const Color GetAmbient() const;
	const Color GetDiffuse() const;
	const Color GetSpecular() const;

LightType GetType() const;

	void Edit();

private:
	LightType mType;

	glm::vec3 mPosition;
	glm::vec3 mDirection;

	Color mAmbientColor;
	Color mDiffuseColor;
	Color mSpecularColor;

	float mConstant;
	float mLinear;
	float mQuadratic;

};
