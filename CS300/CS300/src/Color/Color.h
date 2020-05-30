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

class Color
{
public:

	Color(float r, float g, float b, float a);
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	static Color White;
	static Color Black;

	static Color Red;
	static Color Green;
	static Color Blue;

	static Color Cyan;
	static Color Pink;
	static Color Yellow;

private:

	unsigned char mR;
	unsigned char mG;
	unsigned char mB;
	unsigned char mA;
};