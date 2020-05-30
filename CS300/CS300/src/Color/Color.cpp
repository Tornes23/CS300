/*!**************************************************************************
\file    Color.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 06:48:22 2020

\brief   This file contains the implementation of the Color class and the static variables in it

The functions included are:
- Color::Color(float r, float g, float b, float a);
- Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

***************************************************************************/


#include "Color.h"

//setting the static variables
Color Color::White  = Color(1.0F, 1.0F, 1.0F, 1.0F);
Color Color::Black  = Color(0.0F, 0.0F, 0.0F, 1.0F);
Color Color::Red    = Color(1.0F, 0.0F, 0.0F, 1.0F);
Color Color::Green  = Color(0.0F, 1.0F, 0.0F, 1.0F);
Color Color::Blue   = Color(0.0F, 0.0F, 1.0F, 1.0F);
Color Color::Cyan   = Color(0.0F, 1.0F, 1.0F, 1.0F);
Color Color::Pink   = Color(1.0F, 0.0F, 1.0F, 1.0F);
Color Color::Yellow = Color(1.0F, 1.0F, 0.0F, 1.0F);

/**************************************************************************
*!
\fn     Color::Color

\brief 
 non default constructor which takes floats

\param  float r
the value for the red color

\param  float g
the value for the green color


\param  float b
the value for the blue color


\param  float a
the value for the alpha color


*
**************************************************************************/
Color::Color(float r, float g, float b, float a)
{
	//setting the values
	mR = static_cast<unsigned char>(r) * 255;
	mG = static_cast<unsigned char>(g) * 255;
	mB = static_cast<unsigned char>(b) * 255;
	mA = static_cast<unsigned char>(a) * 255;
}

/**************************************************************************
*!
\fn     Color::Color

\brief 
 non default constructor which takes unsigned chars


\param  unsigned char r
the value for the red color


\param  unsigned char g
the value for the green color


\param  unsigned char b
the value for the blue color


\param  unsigned char a
the value for the alpha color


*
**************************************************************************/
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	//setting the values
	mR = r;
	mG = g;
	mB = b;
	mA = a;
}

