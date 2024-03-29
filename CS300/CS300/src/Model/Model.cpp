/*!**************************************************************************
\file    Model.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 07:09:50 2020

\brief   This file contains the 
implementation of the model class

The functions included are:
- Model::Model(Shape form, int precision);
- void Model::UpdateModel(int modification);
- void Model::CreateSphere(int slices, float radius);
- void Model::CreateCylinder(int slices, float radius, float height);
- void Model::CreateCone(int slices, float radius, float height);
- void Model::CreateCube();
- void Model::CreatePlane();
- void Model::BindBuffers();
- void Model::BindModelBuffer();
- void Model::BindNormalBuffer();
- void Model::BindAverageBuffer();
- void Model::GenBuffers();
- void Model::FreeBuffers();
- GLsizei Model::GetDrawElements() const;
- GLsizei Model::GetNormalCount() const;
- const GLuint Model::GetVAO() const;
- const GLuint Model::GetNormalVAO() const;
- void Model::GenNormals()
- void Model::GenTangents()

***************************************************************************/


#include "Model.h"
#include <GLM/gtc/constants.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/normal.hpp>
#include <iostream>

/**************************************************************************
*!
\fn     Model::Model

\brief 
The default constructor of the class

\param  Shape form
The shape of the model

\param  int precision
The precision the model will have in case it contains a circular shape

*
**************************************************************************/
Model::Model(Shape form, int precision)
{
	//setting the member variables
	mPrecision = precision;
	mShape = form;
	mIndexed = true;

	//depending the given shape call to create it
	switch (mShape)
	{
	case Model::Cube:
		CreateCube();
		break;
	case Model::Cylinder:
		CreateCylinder(mPrecision, 0.5F);
		break;
	case Model::Cone:
		CreateCone(mPrecision, 0.5F);
		break;
	case Model::Sphere:
		CreateSphere(mPrecision, 0.5F);
		break;
	case Model::Plane:
		CreatePlane();
		break;
	default:
		break;
	}

}

/**************************************************************************
*!
\fn     Model::UpdateModel

\brief 
Updates the model precision given a value

\param  int modification
the value to add to the precision

*
**************************************************************************/
void Model::UpdateModel(int modification)
{
	//is the value is in boundaries
	if((mPrecision + modification) <= 500 && (mPrecision + modification) >= 3)
		mPrecision += modification;//modify the value

	//calling to recompute the model
	switch (mShape)
	{
	case Model::Cylinder:
		FreeBuffers();

		CreateCylinder(mPrecision, 0.5);
		break;
	case Model::Cone:
		FreeBuffers();

		CreateCone(mPrecision, 0.5);
		break;
	case Model::Sphere:
		FreeBuffers();

		CreateSphere(mPrecision, 0.5);
		break;
	default:
		break;
	}
}

/**************************************************************************
*!
\fn     Model::CreateSphere

\brief
Generates a spherical model

\param  int slices
the number of vertical cuts

\param  float radius
the radius of the sphere

*
**************************************************************************/
void Model::CreateSphere(int slices, float radius)
{
	mIndexed = false;

	//constants used later
	int ringCount = 2 * slices;
	float twoPI = glm::two_pi<float>();
	float pi = glm::pi<float>();
	float piOver2 = glm::half_pi<float>();

	//the increment of the angle for the cosine and sine in the horizontal and verical
	float sliceAngleStep = twoPI / slices;
	float ringAngleStep = pi / ringCount;

	//computing vertices
	for (int i = 0; i < ringCount; ++i)
	{
		//the current angle
		float ringAngle = piOver2 - i * ringAngleStep;
		float nextRingAngle = piOver2 - (i + 1) * ringAngleStep;

		for (int j = 0; j < slices; ++j)
		{
			//the current angle
			float sliceAngle = j * sliceAngleStep;
			float nextSliceAngle = (j + 1) * sliceAngleStep;

			glm::vec3 v0;
			glm::vec3 v1;
			glm::vec3 v2;

			//computing the x y z coords using the sphere parametric formula
			v0.x = (radius * cosf(ringAngle)) * sinf(sliceAngle);
			v0.y = (radius * sinf(ringAngle));
			v0.z = (radius * cosf(ringAngle)) * cosf(sliceAngle);

			v1.x = (radius * cosf(nextRingAngle)) * sinf(sliceAngle);
			v1.y = (radius * sinf(nextRingAngle));
			v1.z = (radius * cosf(nextRingAngle)) * cosf(sliceAngle);

			v2.x = (radius * cosf(ringAngle)) * sinf(nextSliceAngle);
			v2.y = (radius * sinf(ringAngle));
			v2.z = (radius * cosf(ringAngle)) * cosf(nextSliceAngle);

			//computing the u v coordinates
			float s = static_cast<float>(j) / slices;
			float sNext = static_cast<float>(j + 1) / slices;

			float t = static_cast<float>(i) / ringCount;
			float tNext = static_cast<float>(i + 1) / ringCount;

			//adding the position texture coordinates and normal values for the first triangle
			mVertices.push_back(v0);
			mVertices.push_back(v1);
			mVertices.push_back(v2);

			mAveraged.push_back(v0);
			mAveraged.push_back(v1);
			mAveraged.push_back(v2);

			mTextureCoords.push_back(glm::vec2(s, t));
			mTextureCoords.push_back(glm::vec2(s, tNext));
			mTextureCoords.push_back(glm::vec2(sNext, t));
			

			//adding the position texture coordinates and normal values for the second triangle
			v0.x = (radius * cosf(nextRingAngle)) * sinf(nextSliceAngle);
			v0.y = (radius * sinf(nextRingAngle));
			v0.z = (radius * cosf(nextRingAngle)) * cosf(nextSliceAngle);
			
			mVertices.push_back(v0);
			mVertices.push_back(v2);
			mVertices.push_back(v1);
			
			mAveraged.push_back(v0);
			mAveraged.push_back(v2);
			mAveraged.push_back(v1);
			
			mTextureCoords.push_back(glm::vec2(sNext, tNext));
			mTextureCoords.push_back(glm::vec2(sNext, t));
			mTextureCoords.push_back(glm::vec2(s, tNext));
			
		}
	}

	GenNormals();

	GenTangents();

	//calling to bind the buffers
	BindBuffers();
}

/**************************************************************************
*!
\fn     Model::CreateCylinder

\brief
Generates a cylindrical model

\param  int slices
The number of division that the top and bottom circles will have

\param  float radius
the radius of the cylinder

\param  float height
the height of the cylinder

*
**************************************************************************/
void Model::CreateCylinder(int slices, float radius, float height)
{
	mIndexed = false;

	//vector to store the vertices of a unit circle
	std::vector<float> circleCoords;

	//constants used later
	float twoPi = glm::two_pi<float>();
	float angleStep = twoPi / slices;
	float botY = -height / 2.0F;
	float topY = height / 2.0F;

	//computing the coordinates of the circle
	for (int i = 0; i < slices; i++)
	{
		float angle = i * angleStep;
		circleCoords.push_back(cosf(angle));
		circleCoords.push_back(0);
		circleCoords.push_back(sinf(angle));
	}

	int k = 0;
	//middle triangles
	for (int i = 0; i < slices; i++)
	{
		//getting the x y z coordinates of the circle
		float xCoord = circleCoords[k];
		float yCoord = circleCoords[k + 1];
		float zCoord = circleCoords[k + 2];

		float xCoord2;
		float yCoord2;
		float zCoord2;

		if (i < slices - 1)
		{
			//getting the x y z coordinates of the circle
			xCoord2 = circleCoords[k + 3];
			yCoord2 = circleCoords[k + 4];
			zCoord2 = circleCoords[k + 5];
		}
		else
		{
			//getting the x y z coordinates of the circle
			xCoord2 = circleCoords[0];
			yCoord2 = circleCoords[1];
			zCoord2 = circleCoords[2];
		}

#pragma region MIDDLE TRIANGLES

		glm::vec3 v0 = glm::vec3(xCoord2 * radius, botY, zCoord2 * radius);
		glm::vec3 v1 = glm::vec3(xCoord * radius, botY, zCoord * radius);
		glm::vec3 v2 = glm::vec3(xCoord * radius, topY, zCoord * radius);

#pragma region TRIANGLE 1
		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v0);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
		mAveraged.push_back(v0);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v1);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));
		mAveraged.push_back(v1);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v2);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 1));
		mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

		v0 = glm::vec3(xCoord * radius, topY, zCoord * radius);
		v1 = glm::vec3(xCoord2 * radius, topY, zCoord2 * radius);
		v2 = glm::vec3(xCoord2 * radius, botY, zCoord2 * radius);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v0);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 1));
		mAveraged.push_back(v0);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v1);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 1));
		mAveraged.push_back(v1);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v2);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
		mAveraged.push_back(v2);
#pragma endregion

#pragma endregion 

#pragma region TOP AND BOT TRIANGLES

#pragma region TOP TRIANGLE

		v0 = glm::vec3(xCoord * radius, topY, zCoord * radius);
		v1 = glm::vec3(0, topY, 0);
		v2 = glm::vec3(xCoord2 * radius, topY, zCoord2 * radius);

		float uMid = (static_cast<float>(i) / slices + static_cast<float>(i + 1) / slices) / 2.0F;

		mVertices.push_back(v0);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 1));
		mAveraged.push_back(v0);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v1);
		mTextureCoords.push_back(glm::vec2(uMid, 1));
		mAveraged.push_back(v1);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v2);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 1));
		mAveraged.push_back(v2);
#pragma endregion

#pragma region BOT TRIANGLE

		v0 = glm::vec3(xCoord2 * radius, botY, zCoord2 * radius);
		v1 = glm::vec3(0, botY, 0);
		v2 = glm::vec3(xCoord * radius, botY, zCoord * radius);

		mVertices.push_back(v0);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
		mAveraged.push_back(v0);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v1);
		mTextureCoords.push_back(glm::vec2(uMid, 0));
		mAveraged.push_back(v1);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v2);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));
		mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

		k += 3;
	}

	GenNormals();

	GenTangents();

	//binding the buffers
	BindBuffers();
}

/**************************************************************************
*!
\fn     Model::CreateCone

\brief
Generates a conical model

\param  int slices
The number of division that the bottom circle will have

\param  float radius
the radius of the cone

\param  float height
the height of the cylinder

*
**************************************************************************/
void Model::CreateCone(int slices, float radius, float height)
{
	mIndexed = false;

	//vector to store the vertices of a unit circle
	std::vector<float> circleCoords;

	//constants used later
	float twoPi = glm::two_pi<float>();
	float angleStep = twoPi / slices;
	float botY = -height / 2.0F;
	float topY = height / 2.0F;

	//computing the coordinates of the circle
	for (int i = 0; i < slices; i++)
	{
		float angle = i * angleStep;
		circleCoords.push_back(cosf(angle));
		circleCoords.push_back(0);
		circleCoords.push_back(sinf(angle));
	}

	//variable to loop throught the circle vector
	int k = 0;

	//computing triangles
	for (int i = 0; i < slices; i++)
	{
		float angle = i * angleStep;
		float angle2 = (i + 1) * angleStep;
		glm::mat4x4 rotationMat(1.0F); 

		rotationMat = glm::rotate(rotationMat, -angle, glm::vec3(0, 1, 0));

		glm::mat4x4 rotationMat2(1.0F);

		rotationMat2 = glm::rotate(rotationMat2, -angle2, glm::vec3(0, 1, 0));;

		//getting the x y z coordinates of the circle
		float xCoord = circleCoords[k];
		float yCoord = circleCoords[k + 1];
		float zCoord = circleCoords[k + 2];

		float uMid = (static_cast<float>(i) / slices + static_cast<float>(i + 1) / slices) / 2.0F;

		float xCoord2;
		float yCoord2;
		float zCoord2;

		if (i < slices - 1)
		{
			//getting the x y z coordinates of the circle
			xCoord2 = circleCoords[k + 3];
			yCoord2 = circleCoords[k + 4];
			zCoord2 = circleCoords[k + 5];
		}
		else
		{
			//getting the x y z coordinates of the circle
			xCoord2 = circleCoords[0];
			yCoord2 = circleCoords[1];
			zCoord2 = circleCoords[2];
		}

		glm::vec3 v0 = glm::vec3(xCoord * radius, botY, zCoord * radius);
		glm::vec3 v1 = glm::vec3(xCoord2 * radius, botY, zCoord2 * radius);
		glm::vec3 v2 = glm::vec3(0, topY, 0);

		glm::vec3 xVec = glm::vec3(1, 0, 0);
		glm::vec3 yVec = glm::vec3(0, -1, 0);
		glm::vec3 zVec = glm::vec3(0, 0.5, 0);

		glm::vec3 result = xVec + zVec;

		result = yVec + normalize(result);

		result /= 2;

		glm::vec3 average = normalize(result);

#pragma region MIDDLE TRIANGLE
		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v1);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
		mAveraged.push_back(glm::vec3(rotationMat2 * glm::vec4(average, 1.0)));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v0);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0)); 
		mAveraged.push_back(glm::vec3(rotationMat * glm::vec4(average, 1.0)));


		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v2);
		mTextureCoords.push_back(glm::vec2(uMid, 1));
		mAveraged.push_back(glm::vec3(0, 1, 0));

#pragma endregion

#pragma region BOTTOM TRIANGLE

		v2 = glm::vec3(0, botY, 0);

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v2);
		mTextureCoords.push_back(glm::vec2(uMid, 0));
		mAveraged.push_back(glm::vec3(0, -1, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v0);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));
		mAveraged.push_back(rotationMat * glm::vec4(average, 1.0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(v1);
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
		mAveraged.push_back(rotationMat2 * glm::vec4(average, 1.0));

#pragma endregion

		k += 3;
	}

	GenNormals();

	GenTangents();

	//calling to bind the buffers
	BindBuffers();
}

/**************************************************************************
*!
\fn     Model::CreateCube

\brief
Generates a cubical model

*
**************************************************************************/
void Model::CreateCube()
{
	mIndexed = false;

	//front
	//adding the position texture coordinates and normal values for the four vertices
	glm::vec3 v0 = glm::vec3(-0.5, -0.5, 0.5);
	glm::vec3 v1 = glm::vec3(0.5, -0.5, 0.5);
	glm::vec3 v2 = glm::vec3(0.5, 0.5, 0.5);

	glm::vec2 uv0 = glm::vec2(0, 0);
	glm::vec2 uv1 = glm::vec2(1, 0);
	glm::vec2 uv2 = glm::vec2(1, 1);
	glm::vec2 uv3 = glm::vec2(0, 1);

#pragma region FRONT

#pragma region TRIANGLE 1

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv0);
	mTextureCoords.push_back(uv1);
	mTextureCoords.push_back(uv2);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

	v0 = glm::vec3(0.5, 0.5, 0.5);
	v1 = glm::vec3(-0.5, 0.5, 0.5);
	v2 = glm::vec3(-0.5, -0.5, 0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv2);
	mTextureCoords.push_back(uv3);
	mTextureCoords.push_back(uv0);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

#pragma region RIGHT

#pragma region TRIANGLE 1
	//adding the position texture coordinates and normal values for the four vertices
	v0 = glm::vec3(0.5, -0.5, 0.5);
	v1 = glm::vec3(0.5, -0.5, -0.5);
	v2 = glm::vec3(0.5, 0.5, -0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv0);
	mTextureCoords.push_back(uv1);
	mTextureCoords.push_back(uv2);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

	v0 = glm::vec3(0.5, 0.5, -0.5);
	v1 = glm::vec3(0.5, 0.5, 0.5);
	v2 = glm::vec3(0.5, -0.5, 0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv2);
	mTextureCoords.push_back(uv3);
	mTextureCoords.push_back(uv0);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

#pragma region LEFT

#pragma region TRIANGLE 1
	//adding the position texture coordinates and normal values for the four vertices
	v0 = glm::vec3(-0.5, -0.5, -0.5);
	v1 = glm::vec3(-0.5, -0.5,  0.5);
	v2 = glm::vec3(-0.5,  0.5,  0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv0);
	mTextureCoords.push_back(uv1);
	mTextureCoords.push_back(uv2);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

	v0 = glm::vec3(-0.5,  0.5,  0.5);
	v1 = glm::vec3(-0.5,  0.5, -0.5);
	v2 = glm::vec3(-0.5, -0.5, -0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv2);
	mTextureCoords.push_back(uv3);
	mTextureCoords.push_back(uv0);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

#pragma region BACK

#pragma region TRIANGLE 1
	//adding the position texture coordinates and normal values for the four vertices
	v0 = glm::vec3(-0.5, 0.5, -0.5);
	v1 = glm::vec3(0.5, 0.5, -0.5);
	v2 = glm::vec3(0.5, -0.5, -0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv0);
	mTextureCoords.push_back(uv1);
	mTextureCoords.push_back(uv2);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

	v0 = glm::vec3(0.5, -0.5, -0.5);
	v1 = glm::vec3(-0.5, -0.5, -0.5);
	v2 = glm::vec3(-0.5, 0.5, -0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv2);
	mTextureCoords.push_back(uv3);
	mTextureCoords.push_back(uv0);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

#pragma region BOT

#pragma region TRIANGLE 1
	//adding the position texture coordinates and normal values for the four vertices
	v0 = glm::vec3(0.5, -0.5, -0.5);
	v1 = glm::vec3(0.5, -0.5, 0.5);
	v2 = glm::vec3(-0.5, -0.5, 0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);
	
	mTextureCoords.push_back(uv0);
	mTextureCoords.push_back(uv1);
	mTextureCoords.push_back(uv2);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

	v0 = glm::vec3(-0.5, -0.5, 0.5);
	v1 = glm::vec3(-0.5, -0.5, -0.5);
	v2 = glm::vec3(0.5, -0.5, -0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv2);
	mTextureCoords.push_back(uv3);
	mTextureCoords.push_back(uv0);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

#pragma region TOP

#pragma region TRIANGLE 1

	//adding the position texture coordinates and normal values for the four vertices
	v0 = glm::vec3(0.5, 0.5, 0.5);
	v1 = glm::vec3(0.5, 0.5, -0.5);
	v2 = glm::vec3(-0.5, 0.5, -0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv0);
	mTextureCoords.push_back(uv1);
	mTextureCoords.push_back(uv2);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma region TRIANGLE 2

	v0 = glm::vec3(-0.5, 0.5, -0.5);
	v1 = glm::vec3(-0.5, 0.5, 0.5);
	v2 = glm::vec3(0.5, 0.5, 0.5);

	mVertices.push_back(v0);
	mVertices.push_back(v1);
	mVertices.push_back(v2);

	mTextureCoords.push_back(uv2);
	mTextureCoords.push_back(uv3);
	mTextureCoords.push_back(uv0);

	mAveraged.push_back(v0);
	mAveraged.push_back(v1);
	mAveraged.push_back(v2);

#pragma endregion

#pragma endregion

	GenNormals();

	GenTangents();

	//binding buffers
	BindBuffers();
}

/**************************************************************************
*!
\fn     Model::CreatePlane

\brief
Generates a plane model

*
**************************************************************************/
void Model::CreatePlane()
{
	mIndexed = false;
	glm::vec3 v0 = glm::vec3(-0.5, -0.5, 0.0);
	glm::vec3 v1 = glm::vec3(0.5, -0.5, 0.0);
	glm::vec3 v2 = glm::vec3(0.5, 0.5, 0.0);
	glm::vec3 v3 = glm::vec3(-0.5, 0.5, 0.0);

	glm::vec2 uv0 = glm::vec2(0, 0);
	glm::vec2 uv1 = glm::vec2(1, 0);
	glm::vec2 uv2 = glm::vec2(1, 1);
	glm::vec2 uv3 = glm::vec2(0, 1);

	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(v0);
	mTextureCoords.push_back(uv0);
	mAveraged.push_back(glm::triangleNormal(v0, v1, v2));

	mVertices.push_back(v1);
	mTextureCoords.push_back(uv1);
	mAveraged.push_back(glm::triangleNormal(v0, v1, v2));

	mVertices.push_back(v2);
	mTextureCoords.push_back(uv2);
	mAveraged.push_back(glm::triangleNormal(v0, v1, v2));

	//TRIANGLE 2

	mVertices.push_back(v2);
	mTextureCoords.push_back(uv2);
	mAveraged.push_back(glm::triangleNormal(v3, v0, v2));

	mVertices.push_back(v3);
	mTextureCoords.push_back(uv3);
	mAveraged.push_back(glm::triangleNormal(v3, v0, v2));

	mVertices.push_back(v0);
	mTextureCoords.push_back(uv0);
	mAveraged.push_back(glm::triangleNormal(v3, v0, v2));

	GenNormals();

	GenTangents();

	BindBuffers();
}

/**************************************************************************
*!
\fn     Model::BindBuffers

\brief 
Binds the VBO and VAO buffers of the model

*
**************************************************************************/
void Model::BindBuffers()
{
	GenBuffers();

	BindModelBuffer();

	BindNormalBuffer();
}

/**************************************************************************
*!
\fn     Model::BindNormalBuffer

\brief
Binds the VBO and VAO buffers of the normals

*
**************************************************************************/
void Model::BindNormalBuffer()
{
	//binding the VAO
	glBindVertexArray(mVAO[1]);

	//addign pos
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//addign normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, mNormalVecs.size() * sizeof(glm::vec3), &mNormalVecs[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//addign average normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, mAveraged.size() * sizeof(glm::vec3), &mAveraged[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, mTangents.size() * sizeof(glm::vec3), &mTangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[5]);
	glBufferData(GL_ARRAY_BUFFER, mAvgTangents.size() * sizeof(glm::vec3), &mAvgTangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[6]);
	glBufferData(GL_ARRAY_BUFFER, mBitangents.size() * sizeof(glm::vec3), &mBitangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[7]);
	glBufferData(GL_ARRAY_BUFFER, mAvgBitangents.size() * sizeof(glm::vec3), &mAvgBitangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	if (mIndexed)
	{
		//adding triangle indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[8]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(unsigned short), &mIndexes[0], GL_STATIC_DRAW);
	}

	// Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(mVAO[1]);
}

/**************************************************************************
*!
\fn     Model::BindModelBuffer

\brief
Binds the VBO and VAO buffers of the model

*
**************************************************************************/
void Model::BindModelBuffer()
{
	//binding the VAO
	glBindVertexArray(mVAO[0]);

	//addign pos
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding textureCoords
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, mTextureCoords.size() * sizeof(glm::vec2), &mTextureCoords[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	//addign normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, mNormalVecs.size() * sizeof(glm::vec3), &mNormalVecs[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//addign average normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, mAveraged.size() * sizeof(glm::vec3), &mAveraged[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, mTangents.size() * sizeof(glm::vec3), &mTangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[5]);
	glBufferData(GL_ARRAY_BUFFER, mAvgTangents.size() * sizeof(glm::vec3), &mAvgTangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[6]);
	glBufferData(GL_ARRAY_BUFFER, mBitangents.size() * sizeof(glm::vec3), &mBitangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//adding tangents
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[7]);
	glBufferData(GL_ARRAY_BUFFER, mAvgBitangents.size() * sizeof(glm::vec3), &mAvgBitangents[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	if (mIndexed)
	{
		//adding triangle indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[8]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(unsigned short), &mIndexes[0], GL_STATIC_DRAW);
	}

	// Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(mVAO[0]);
}

/**************************************************************************
*!
\fn     Model::FreeBuffers

\brief 
Deallocates the VBOs and VAOs

*
**************************************************************************/
void Model::FreeBuffers()
{
	//deallocating the 2 VAOs and the 4 VBOs
	glDeleteBuffers(9, mVBO);
	glDeleteVertexArrays(2, mVAO);

	//clearing the vectors
	mVertices.clear();
	mNormalVecs.clear();
	mAveraged.clear();
	mTextureCoords.clear();
	mIndexes.clear();
	mTangents.clear();
	mBitangents.clear();
	mAvgTangents.clear();
	mAvgBitangents.clear();

}

/**************************************************************************
*!
\fn     Model::GenBuffers

\brief
Generates the buffers

*
**************************************************************************/
void Model::GenBuffers()
{
	//generating the VAO and VBO buffers
	glGenVertexArrays(2, mVAO);
	glGenBuffers(9, mVBO);
}

/**************************************************************************
*!
\fn     Model::GenNormals

\brief
Generates the normals

*
**************************************************************************/
void Model::GenNormals()
{
	int size = mIndexed ? static_cast<int>(mIndexes.size()) : static_cast<int>(mVertices.size());

	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;

	for (int i = 0; i < size; i += 3)
	{
		if (!mIndexed)
		{
			v0 = mVertices[i + 0];
			v1 = mVertices[i + 1];
			v2 = mVertices[i + 2];
		}
		else
		{
			v0 = mVertices[mIndexes[i + 0]];
			v1 = mVertices[mIndexes[i + 1]];
			v2 = mVertices[mIndexes[i + 2]];
		}

		glm::vec3 normal = glm::normalize(glm::triangleNormal(v0, v1, v2));

		mNormalVecs.push_back(normal);
		mNormalVecs.push_back(normal);
		mNormalVecs.push_back(normal);
	}

}

/**************************************************************************
*!
\fn     Model::GenTangents

\brief
Generates the tangents

*
**************************************************************************/
void Model::GenTangents()
{
	int size = mIndexed ? static_cast<int>(mIndexes.size()) : static_cast<int>(mVertices.size());

	mTangents.resize(mVertices.size(), glm::vec3(0, 0, 0));
	mBitangents.resize(mVertices.size(), glm::vec3(0, 0, 0));

	mAvgTangents.resize(mVertices.size(), glm::vec3(0, 0, 0));
	mAvgBitangents.resize(mVertices.size(), glm::vec3(0, 0, 0));

	//Loop through the triangles
	for (int i = 0; i < size; i += 3)
	{
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;

		glm::vec2 uv0;
		glm::vec2 uv1;
		glm::vec2 uv2;


		if (mIndexed)
		{
			v0 = mVertices[mIndexes[i + 0]];
			v1 = mVertices[mIndexes[i + 1]];
			v2 = mVertices[mIndexes[i + 2]];

			uv0 = mTextureCoords[mIndexes[i + 0]];
			uv1 = mTextureCoords[mIndexes[i + 1]];
			uv2 = mTextureCoords[mIndexes[i + 2]];
		}
		else
		{
			v0 = mVertices[i + 0];
			v1 = mVertices[i + 1];
			v2 = mVertices[i + 2];

			uv0 = mTextureCoords[i + 0];
			uv1 = mTextureCoords[i + 1];
			uv2 = mTextureCoords[i + 2];
		}

		glm::vec3 side1 = v1 - v0;
		glm::vec3 side2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float f = 1.0F;

		if (((deltaUV1.y * deltaUV2.x) - (deltaUV2.y * deltaUV1.x)) != 0.0F)
			f = 1.0F / ((deltaUV1.y * deltaUV2.x) - (deltaUV2.y * deltaUV1.x));

		// Solve equations to find T and B for this triangle
		glm::vec3 T = ((deltaUV1.y * side2) - (deltaUV2.y * side1)) * f;

		glm::vec3 B = ((deltaUV2.x * side1) - (deltaUV1.x * side2)) * f;

		// Accumulate tangent/bitangent for the 3 vertices of the triangle (to average after)
		if (mIndexed)
		{
			mTangents[mIndexes[i + 0]] += T;
			mTangents[mIndexes[i + 1]] += T;
			mTangents[mIndexes[i + 2]] += T;

			mAvgTangents[mIndexes[i + 0]] += T;
			mAvgTangents[mIndexes[i + 1]] += T;
			mAvgTangents[mIndexes[i + 2]] += T;

			mBitangents[mIndexes[i + 0]] += B;
			mBitangents[mIndexes[i + 1]] += B;
			mBitangents[mIndexes[i + 2]] += B;

			mAvgBitangents[mIndexes[i + 0]] += B;
			mAvgBitangents[mIndexes[i + 1]] += B;
			mAvgBitangents[mIndexes[i + 2]] += B;
		}
		else
		{
			mTangents[i + 0] += T;
			mTangents[i + 1] += T;
			mTangents[i + 2] += T;

			mAvgTangents[i + 0] += T;
			mAvgTangents[i + 1] += T;
			mAvgTangents[i + 2] += T;

			mBitangents[i + 0] += B;
			mBitangents[i + 1] += B;
			mBitangents[i + 2] += B;

			mAvgBitangents[i + 0] += B;
			mAvgBitangents[i + 1] += B;
			mAvgBitangents[i + 2] += B;
		}
	}


	// Loop through every vertex
	for (unsigned i = 0; i < mVertices.size(); i++)
	{
		glm::vec3 normal = mNormalVecs[i];
		glm::vec3 tangent = mTangents[i];
		glm::vec3 bitan = mBitangents[i];

		// Gram-Schmidt orthogonalization of tangent respect to normal and normalize tangent
		if (tangent != glm::vec3(0, 0, 0))
			mTangents[i] = glm::normalize(tangent - normal * glm::dot(normal, tangent));
		else
		{
			tangent = glm::vec3(1, 0, 0);
			mTangents[i] = tangent;
		}

		glm::vec3 finalBitan = bitan;

		finalBitan = glm::normalize(glm::cross(normal, tangent));

		// Compute the new perpendicular bitangent maintaining the original handeness of the previously 
		finalBitan = glm::dot(finalBitan, bitan) >= 0 ? finalBitan : -finalBitan;

		// computed one (T,B,N need to be normalized and orthogonal at this point)
		mBitangents[i] = finalBitan;

	}

	GenAvgTangents();
}

/**************************************************************************
*!
\fn     Model::GenAvgTangents

\brief
Generates the average tangents

*
**************************************************************************/
void Model::GenAvgTangents()
{
	SortAvgTangents();
	SortAvgBitangents();

	//for each vertex
	for (unsigned i = 0; i < mVertices.size(); i++)
	{
		//set the average to 0
		glm::vec3 avgNormal = mAveraged[i];
		glm::vec3 avgTangent = mAvgTangents[i];
		glm::vec3 avgBitan = mAvgBitangents[i];


		// Gram-Schmidt orthogonalization of tangent respect to normal and normalize tangent
		if (avgTangent != glm::vec3(0, 0, 0))
			mAvgTangents[i] = glm::normalize(avgTangent - avgNormal * glm::dot(avgNormal, avgTangent));
		else
		{
			avgTangent = glm::vec3(1, 0, 0);
			mAvgTangents[i] = avgTangent;
		}

		glm::vec3 finalBitan = avgBitan;

		finalBitan = glm::normalize(glm::cross(avgNormal, avgTangent));

		// Compute the new perpendicular bitangent maintaining the original handeness of the previously 
		finalBitan = glm::dot(finalBitan, avgBitan) >= 0 ? finalBitan : -finalBitan;

		// computed one (T,B,N need to be normalized and orthogonal at this point)
		mAvgBitangents[i] = finalBitan;
	}
}


/**************************************************************************
*!
\fn     Model::SortAvgTangents

\brief
Sorts the tangents of each vertex if any is 
repeated is set to the first coincidence

*
**************************************************************************/
void Model::SortAvgTangents()
{
	//epsilon value to check the position
	float errorVal = 0.01F;

	//looping throught the vertices
	for (unsigned i = 0; i < mAvgTangents.size(); i++)
	{
		//another loop to compare the vertices
		for (unsigned j = i + 1; j < mAvgTangents.size(); j++)
		{
			//computing the difference between the two vertex
			float differenceX = glm::abs(mVertices[i].x - mVertices[j].x);
			float differenceY = glm::abs(mVertices[i].y - mVertices[j].y);
			float differenceZ = glm::abs(mVertices[i].z - mVertices[j].z);

			//if the difference is lower than the error value
			if (differenceX < errorVal && differenceY < errorVal && differenceZ < errorVal)
			{
				mAvgTangents[i] += mAvgTangents[j];
				mAvgTangents[j] = glm::vec3(0,0,0);
			}
		}

		mAvgTangents[i] = glm::normalize(mAvgTangents[i]);
	}

	for (unsigned i = 0; i < mAvgTangents.size(); i++)
	{
		for (unsigned j = i + 1; j < mAvgTangents.size(); j++)
		{
			//computing the difference between the two vertex
			float differenceX = glm::abs(mVertices[i].x - mVertices[j].x);
			float differenceY = glm::abs(mVertices[i].y - mVertices[j].y);
			float differenceZ = glm::abs(mVertices[i].z - mVertices[j].z);

			//if the difference is lower than the error value
			if (differenceX < errorVal && differenceY < errorVal && differenceZ < errorVal)
				mAvgTangents[j] = mAvgTangents[i];
		}
	}

}

/**************************************************************************
*!
\fn     Model::SortAvgBitangents

\brief
Sorts the bitangents of each vertex if any is
repeated is set to the first coincidence

*
**************************************************************************/
void Model::SortAvgBitangents()
{
	//epsilon value to check the position
	float errorVal = 0.01F;

	//looping throught the vertices
	for (unsigned i = 0; i < mAvgBitangents.size(); i++)
	{
		//another loop to compare the vertices
		for (unsigned j = i + 1; j < mAvgBitangents.size(); j++)
		{
			//computing the difference between the two vertex
			float differenceX = glm::abs(mVertices[i].x - mVertices[j].x);
			float differenceY = glm::abs(mVertices[i].y - mVertices[j].y);
			float differenceZ = glm::abs(mVertices[i].z - mVertices[j].z);

			//if the difference is lower than the error value
			if (differenceX < errorVal && differenceY < errorVal && differenceZ < errorVal)
			{
				mAvgBitangents[i] += mAvgBitangents[j];
				mAvgBitangents[j] = glm::vec3(0, 0, 0);
			}
		}

		mAvgBitangents[i] = glm::normalize(mAvgBitangents[i]);
	}

	for (unsigned i = 0; i < mAvgBitangents.size(); i++)
	{
		for (unsigned j = i + 1; j < mAvgBitangents.size(); j++)
		{
			//computing the difference between the two vertex
			float differenceX = glm::abs(mVertices[i].x - mVertices[j].x);
			float differenceY = glm::abs(mVertices[i].y - mVertices[j].y);
			float differenceZ = glm::abs(mVertices[i].z - mVertices[j].z);

			//if the difference is lower than the error value
			if (differenceX < errorVal && differenceY < errorVal && differenceZ < errorVal)
				mAvgBitangents[j] = mAvgBitangents[i];
		}
	}
}

/**************************************************************************
*!
\fn     Model::GetDrawElements

\brief 
Gets the numeber of indexes

\return GLsizei
the count of indexes to be rendered

*
**************************************************************************/
GLsizei Model::GetDrawElements() const
{
	//returning the index amount
	if(mIndexed)
		return static_cast<GLsizei>(mIndexes.size());
	else
		return static_cast<GLsizei>(mVertices.size());
}

bool Model::GetIndexed()
{
	return mIndexed;
}

/**************************************************************************
*!
\fn     Model::GetVAO

\brief 
Gets the VAO to render

\return const GLuint
Returns the handle for the VAO

*
**************************************************************************/
const GLuint Model::GetVAO() const
{
	//returing the VAO of the model
	return mVAO[0];
}

/**************************************************************************
*!
\fn     Model::GetNormalVAO

\brief
Gets the VAO to render

\return const GLuint
Returns the handle for the VAO

*
**************************************************************************/
const GLuint Model::GetNormalVAO() const
{
	//returing the VAO of the normal
	return mVAO[1];
}

Model::Shape Model::GetShape() const
{
	return mShape;
}

void Model::SetShape(Shape shape)
{
	mShape = shape;

	FreeBuffers();

	switch (mShape)
	{
	case Model::Cube:
		mIndexed = false;
		CreateCube();
		break;
	case Model::Cylinder:
		mIndexed = false;
		CreateCylinder(mPrecision, 0.5F);
		break;
	case Model::Cone:
		mIndexed = false;
		CreateCone(mPrecision, 0.5F);
		break;
	case Model::Sphere:
		mIndexed = true;
		CreateSphere(mPrecision, 0.5F);
		break;
	case Model::Plane:
		mIndexed = true;
		CreatePlane();
		break;
	default:
		break;
	}
}



