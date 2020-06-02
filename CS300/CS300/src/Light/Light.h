#pragma once
#include <GLM/vec3.hpp>
#include "../Color/Color.h"

class Light
{
public:

	enum LightType
	{
		Point,
		Directional,
		Spotlight
	};

	Light(glm::vec3 position = glm::vec3(0,0,0), glm::vec3 direction = glm::vec3(0, 0, 1), Color ambient = Color::White, Color diffuse = Color::White, Color specular = Color::White, LightType type = LightType::Point);

	const glm::vec3 GetPosition() const;
	const glm::vec3 GetDirection() const;

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

};
