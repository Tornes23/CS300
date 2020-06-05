/*!**************************************************************************
\file    Color.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 06:48:22 2020

\brief   This file contains the definition of the Color class and the static variables in it

The functions included are:
- Color::Color(float r, float g, float b, float a);
- Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

***************************************************************************/
#pragma once
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

class Color
{
public:

	Color(float r = 1.0F, float g = 1.0F, float b = 1.0F, float a = 1.0F);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	static Color White;
	static Color Black;

	static Color Red;
	static Color Green;
	static Color Blue;

	static Color Cyan;
	static Color Pink;
	static Color Yellow;

	void SetColor(glm::vec4& color);
	glm::vec3 GetColor();

	unsigned char mR;
	unsigned char mG;
	unsigned char mB;
	unsigned char mA;
};