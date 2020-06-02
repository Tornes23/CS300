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
- void Model::ComputeNormals()
- void Model::ComputeAverage()

***************************************************************************/


#include "Model.h"
#include <GLM/gtc/constants.hpp>
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
		CreateCylinder(mPrecision);
		break;
	case Model::Cone:
		CreateCone(mPrecision);
		break;
	case Model::Sphere:
		CreateSphere(mPrecision);
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

		CreateCylinder(mPrecision);
		break;
	case Model::Cone:
		FreeBuffers();

		CreateCone(mPrecision);
		break;
	case Model::Sphere:
		FreeBuffers();

		CreateSphere(mPrecision);
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

based on the info provided in the http://www.songho.ca/opengl/gl_sphere.html webpage

*
**************************************************************************/
void Model::CreateSphere(int slices, float radius)
{
	//constants used later
	int ringCount = 2 * slices;
	float twoPI = glm::two_pi<float>();
	float pi = glm::pi<float>();
	float piOver2 = glm::half_pi<float>();

	//the increment of the angle for the cosine and sine in the horizontal and verical
	float sliceAngleStep = twoPI / slices;
	float ringAngleStep = pi / ringCount;

	//computing vertices
	for (int i = 0; i <= ringCount; ++i)
	{
		//the current angle
		float ringAngle = piOver2 - i * ringAngleStep;

		for (int j = 0; j <= slices; ++j)
		{
			//the current angle
			float sliceAngle = j * sliceAngleStep;

			//computing the x y z coords using the sphere parametric formula
			float xCoord = (radius * cosf(ringAngle)) * sinf(sliceAngle);
			float yCoord = (radius * sinf(ringAngle));
			float zCoord = (radius * cosf(ringAngle)) * cosf(sliceAngle);

			//computing the u v coordinates
			float u = static_cast<float>(j) / slices;
			float v = static_cast<float>(i) / ringCount;

			//adding the position texture coordinates and normal values for the four vertices
			mVertices.push_back(glm::vec3(xCoord, yCoord, zCoord));
			mTextureCoords.push_back(glm::vec2(u, v));

		}
	}

	//adding indexes
	for (int i = 0; i < ringCount; ++i)
	{
		//computing the indexes for the triangles
		int index1 = i * (slices + 1);
		int index2 = index1 + slices + 1;

		for (int j = 0; j < slices; ++j, ++index1, ++index2)
		{
			//adding the indexes for the 2 triangles that will form each piece of the sphere
			if (i != 0)
			{
				mIndexes.push_back(index1);
				mIndexes.push_back(index2);
				mIndexes.push_back(index1 + 1);
			}

			
			if (i != (ringCount - 1))
			{
				mIndexes.push_back(index1 + 1);
				mIndexes.push_back(index2);
				mIndexes.push_back(index2 + 1);
			}
		}
	}

	//computing the normals
	ComputeNormals();
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

#pragma region TRIANGLE 1
		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord2 * radius, botY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord * radius, botY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord * radius, topY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 1));
#pragma endregion

#pragma region TRIANGLE 2
		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord * radius, topY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 1));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord2 * radius, topY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 1));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord2 * radius, botY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
#pragma endregion

#pragma endregion TOP AND BOT TRIANGLES

#pragma region TOP TRIANGLE

		float uMid = (static_cast<float>(i) / slices + static_cast<float>(i + 1) / slices) / 2.0F;

		mVertices.push_back(glm::vec3(xCoord * radius, topY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 1));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(0, topY, 0));
		mTextureCoords.push_back(glm::vec2(uMid, 1));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord2 * radius, topY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 1));

#pragma endregion

#pragma region BOT TRIANGLE

		mVertices.push_back(glm::vec3(xCoord2 * radius, botY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(0, botY, 0));
		mTextureCoords.push_back(glm::vec2(uMid, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord * radius, botY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));

#pragma endregion

#pragma endregion

		k += 3;
	}

	ComputeNormals();
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

#pragma region MIDDLE TRIANGLE
		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord2 * radius, botY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord * radius, botY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(0, topY, 0));
		mTextureCoords.push_back(glm::vec2(uMid, 1));
#pragma endregion

#pragma region BOTTOM TRIANGLE
		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(0, botY, 0));
		mTextureCoords.push_back(glm::vec2(uMid, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord * radius, botY, zCoord * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i) / slices, 0));

		//adding the position texture coordinates and normal values for the four vertices
		mVertices.push_back(glm::vec3(xCoord2 * radius, botY, zCoord2 * radius));
		mTextureCoords.push_back(glm::vec2(static_cast<float>(i + 1) / slices, 0));
#pragma endregion

		k += 3;
	}

	ComputeNormals();
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
	mVertices.push_back(glm::vec3(-0.5, -0.5, 0.5));
	mVertices.push_back(glm::vec3(0.5, -0.5, 0.5));
	mVertices.push_back(glm::vec3(0.5, 0.5, 0.5));

	mVertices.push_back(glm::vec3(0.5, 0.5, 0.5));
	mVertices.push_back(glm::vec3(-0.5, 0.5, 0.5));
	mVertices.push_back(glm::vec3(-0.5, -0.5, 0.5));

	mTextureCoords.push_back(glm::vec2(0, 0));
	mTextureCoords.push_back(glm::vec2(1, 0));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mTextureCoords.push_back(glm::vec2(1, 1));
	mTextureCoords.push_back(glm::vec2(0, 1));
	mTextureCoords.push_back(glm::vec2(0, 0));

	//right
	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(glm::vec3(0.5, -0.5, 0.5));
	mVertices.push_back(glm::vec3(0.5, -0.5, -0.5));
	mVertices.push_back(glm::vec3(0.5, 0.5, -0.5));

	mVertices.push_back(glm::vec3(0.5, 0.5, -0.5));
	mVertices.push_back(glm::vec3(0.5, 0.5, 0.5));
	mVertices.push_back(glm::vec3(0.5, -0.5, 0.5));

	mTextureCoords.push_back(glm::vec2(0, 0));
	mTextureCoords.push_back(glm::vec2(1, 0));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mTextureCoords.push_back(glm::vec2(1, 1));
	mTextureCoords.push_back(glm::vec2(0, 1));
	mTextureCoords.push_back(glm::vec2(0, 0));

	//Left
	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
	mVertices.push_back(glm::vec3(-0.5, -0.5, 0.5));
	mVertices.push_back(glm::vec3(-0.5, 0.5, 0.5));

	mVertices.push_back(glm::vec3(-0.5, 0.5, 0.5));
	mVertices.push_back(glm::vec3(-0.5, 0.5, -0.5));
	mVertices.push_back(glm::vec3(-0.5, -0.5, -0.5));

	mTextureCoords.push_back(glm::vec2(0, 0));
	mTextureCoords.push_back(glm::vec2(1, 0));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mTextureCoords.push_back(glm::vec2(1, 1));
	mTextureCoords.push_back(glm::vec2(0, 1));
	mTextureCoords.push_back(glm::vec2(0, 0));


	//back
	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(glm::vec3(-0.5, 0.5, -0.5));
	mVertices.push_back(glm::vec3(0.5, 0.5, -0.5));
	mVertices.push_back(glm::vec3(0.5, -0.5, -0.5));

	mVertices.push_back(glm::vec3(0.5, -0.5, -0.5));
	mVertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
	mVertices.push_back(glm::vec3(-0.5, 0.5, -0.5));

	mTextureCoords.push_back(glm::vec2(0, 0));
	mTextureCoords.push_back(glm::vec2(1, 0));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mTextureCoords.push_back(glm::vec2(1, 1));
	mTextureCoords.push_back(glm::vec2(0, 1));
	mTextureCoords.push_back(glm::vec2(0, 0));

	//bot
	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(glm::vec3(0.5, -0.5, -0.5));
	mVertices.push_back(glm::vec3(0.5, -0.5, 0.5));
	mVertices.push_back(glm::vec3(-0.5, -0.5, 0.5));

	mVertices.push_back(glm::vec3(-0.5, -0.5, 0.5));
	mVertices.push_back(glm::vec3(-0.5, -0.5, -0.5));
	mVertices.push_back(glm::vec3(0.5, -0.5, -0.5));

	mTextureCoords.push_back(glm::vec2(0, 0));
	mTextureCoords.push_back(glm::vec2(0, 1));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mTextureCoords.push_back(glm::vec2(1, 1));
	mTextureCoords.push_back(glm::vec2(1, 0));
	mTextureCoords.push_back(glm::vec2(0, 0));

	//top
	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(glm::vec3(0.5, 0.5, 0.5));
	mVertices.push_back(glm::vec3(0.5, 0.5, -0.5));
	mVertices.push_back(glm::vec3(-0.5, 0.5, -0.5));

	mVertices.push_back(glm::vec3(-0.5, 0.5, -0.5));
	mVertices.push_back(glm::vec3(-0.5, 0.5, 0.5));
	mVertices.push_back(glm::vec3(0.5, 0.5, 0.5));

	mTextureCoords.push_back(glm::vec2(0, 0));
	mTextureCoords.push_back(glm::vec2(1, 0));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mTextureCoords.push_back(glm::vec2(1, 1));
	mTextureCoords.push_back(glm::vec2(0, 1));
	mTextureCoords.push_back(glm::vec2(0, 0));

	ComputeNormals();
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
	//adding the position texture coordinates and normal values for the four vertices
	mVertices.push_back(glm::vec3(-0.5, -0.5, 0.0));
	mTextureCoords.push_back(glm::vec2(0, 0));

	mVertices.push_back(glm::vec3(0.5, -0.5, 0.0));
	mTextureCoords.push_back(glm::vec2(1, 0));

	mVertices.push_back(glm::vec3(0.5, 0.5, 0.0));
	mTextureCoords.push_back(glm::vec2(1, 1));

	mVertices.push_back(glm::vec3(-0.5, 0.5, 0.0));
	mTextureCoords.push_back(glm::vec2(0, 1));

	//adding the indexes to create 2 triangles
	mIndexes.push_back(0);
	mIndexes.push_back(1);
	mIndexes.push_back(2);

	mIndexes.push_back(2);
	mIndexes.push_back(3);
	mIndexes.push_back(0);

	ComputeNormals();

	BindBuffers();
}

/**************************************************************************
*!
\fn     Model::ComputeNormals

\brief
Generates the normals

*
**************************************************************************/
void Model::ComputeNormals()
{
	size_t size;

	//resizing the vector vector to match the vertex size
	mNormalsPerFace.resize(mVertices.size());

	//if is not using indexes the size is one third of the vertices (3 vertices make a face)
	if (!mIndexed)
		size = mVertices.size() / 3;
	else
		size = mIndexes.size() / 3;

	//variable to access the container
	int k = 0;

	//looping through the faces
	for (size_t i = 0; i < size; i++)
	{
		//if is indexed
		if (mIndexed)
		{
			//computing the normal
			glm::vec3 normal = glm::triangleNormal(mVertices[mIndexes[k]], mVertices[mIndexes[k + 1]], mVertices[mIndexes[k + 2]]);

			//adding it to the corresponding vertices
			mNormalsPerFace[mIndexes[k]].push_back(normal);
			mNormalsPerFace[mIndexes[k + 1]].push_back(normal);
			mNormalsPerFace[mIndexes[k + 2]].push_back(normal);

			//pushing the normals
			mNormals.push_back(mVertices[mIndexes[k]]);
			mNormals.push_back(mVertices[mIndexes[k]] + normal);

			mNormals.push_back(mVertices[mIndexes[k + 1]]);
			mNormals.push_back(mVertices[mIndexes[k + 1]] + normal);

			mNormals.push_back(mVertices[mIndexes[k + 2]]);
			mNormals.push_back(mVertices[mIndexes[k + 2]] + normal);
		}
		else
		{
			//computing the normal
			glm::vec3 normal = glm::triangleNormal(mVertices[k], mVertices[k + 1], mVertices[k + 2]);

			//adding it to the corresponding vertices
			mNormalsPerFace[k].push_back(normal);
			mNormalsPerFace[k + 1].push_back(normal);
			mNormalsPerFace[k + 2].push_back(normal);

			//pushing the normals
			mNormals.push_back(mVertices[k]);
			mNormals.push_back(mVertices[k] + normal);

			mNormals.push_back(mVertices[k + 1]);
			mNormals.push_back(mVertices[k + 1] + normal);

			mNormals.push_back(mVertices[k + 2]);
			mNormals.push_back(mVertices[k + 2] + normal);
		}

		k += 3;
	}

	//computing the average normals 
	ComputeAverage();

}

/**************************************************************************
*!
\fn     Model::ComputeAverage

\brief
Generates the averaged normals

*
**************************************************************************/
void Model::ComputeAverage()
{
	//vector to contain the averaged normal
	glm::vec3 average;

	//epsilon value to check the position
	float errorVal = 0.1F;

	//looping throught the vertices
	for (unsigned i = 0; i < mNormalsPerFace.size(); i++)
	{
		//another loop to compare the vertices
		for (unsigned j = i + 1; j < mNormalsPerFace.size(); j++)
		{
			//computing the difference between the two vertex
			float differenceX = glm::abs(mVertices[i].x - mVertices[j].x);
			float differenceY = glm::abs(mVertices[i].y - mVertices[j].y);
			float differenceZ = glm::abs(mVertices[i].z - mVertices[j].z);

			//if the difference is lower than the error value
			if (differenceX < errorVal && differenceY < errorVal && differenceZ < errorVal)
			{
				//copy the normals to the original vertex
				for (unsigned k = 0; k < mNormalsPerFace[j].size(); k++)
				{
					mNormalsPerFace[i].push_back(mNormalsPerFace[j][k]);
				}
				//clearing the normals of te duplicated vertex
				mNormalsPerFace[j].clear();
			}
		}
	}

	//for each vertex
	for (unsigned i = 0; i < mVertices.size(); i++)
	{
		//set the average to 0
		average = glm::vec3(0, 0, 0);

		//if is empty skip it
		if (mNormalsPerFace[i].empty())
			continue;

		//adding all the normals
		for (unsigned j = 0; j < mNormalsPerFace[i].size(); j++)
		{
			average += mNormalsPerFace[i][j];
		}

		//dividing it over the added amount
		average /= mNormalsPerFace[i].size();

		//normalizing it
		average = glm::normalize(average);

		//adding the normal to the  container
		mAveraged.push_back(mVertices[i]);
		mAveraged.push_back(mVertices[i] + average);
	}
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

	BindAverageBuffer();

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
	glBindVertexArray(mVAO[1]);

	//addign normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(mVAO[1]);
}

/**************************************************************************
*!
\fn     Model::BindAverageBuffer

\brief
Binds the VBO and VAO buffers of the averaged normals

*
**************************************************************************/
void Model::BindAverageBuffer()
{
	glBindVertexArray(mVAO[2]);

	//addign average normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, mAveraged.size() * sizeof(glm::vec3), &mAveraged[0].x, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Unbind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(mVAO[2]);
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
	//binding the VAo
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

	if (mIndexed)
	{
		//adding triangle indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[2]);
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
	glDeleteBuffers(5, mVBO);
	glDeleteVertexArrays(3, mVAO);

	//clearing the vectors
	mVertices.clear();
	mNormals.clear();
	mAveraged.clear();
	mTextureCoords.clear();
	mIndexes.clear();
	mNormalsPerFace.clear();
}

void Model::GenBuffers()
{
	//generating the VAO and VBO buffers
	glGenVertexArrays(3, mVAO);
	glGenBuffers(5, mVBO);
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

/**************************************************************************
*!
\fn     Model::GetNormalCount

\brief 


\return GLsizei


*
**************************************************************************/
GLsizei Model::GetNormalCount(bool average) const
{
	if(average)
		return static_cast<GLsizei>(mAveraged.size());

	return static_cast<GLsizei>(mNormals.size());
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

const GLuint Model::GetNormalVAO(bool average) const
{
	if (average)
		return mVAO[2];

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
		CreateCylinder(mPrecision);
		break;
	case Model::Cone:
		mIndexed = false;
		CreateCone(mPrecision);
		break;
	case Model::Sphere:
		mIndexed = true;
		CreateSphere(mPrecision);
		break;
	case Model::Plane:
		mIndexed = true;
		CreatePlane();
		break;
	default:
		break;
	}
}



