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

	Light(glm::vec3 position, glm::vec3 direction, Color ambient, Color diffuse, Color specular, LightType type);

	const glm::vec3 GetPosition() const;
	const glm::vec3 GetDirection() const;

	const Color GetAmbient() const;
	const Color GetDiffuse() const;
	const Color GetSpecular() const;

	const LightType GetType() const;

private:
	LightType mType;

	glm::vec3 mPosition;
	glm::vec3 mDirection;

	Color mAmbientColor;
	Color mDiffuseColor;
	Color mSpecularColor;

};
