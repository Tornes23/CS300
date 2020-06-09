/*!**************************************************************************
\file    GameObject.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 06:57:10 2020

\brief   This file contains the 
implementation for the game object class

The functions included are:
- GameObject::GameObject(glm::vec3 position, glm::vec3 scale, const char* name, Model::Shape shape
						, std::string textureFile,  bool active);
- glm::mat4x4 GameObject::GenerateM2W();
- glm::mat4x4 GameObject::GenerateOrthogonal();
- glm::mat4x4 GameObject::GenerateInverseM2W();
- glm::mat4x4 GameObject::GenerateAxisAngleMetod(Axis rotationAxis, float amount);
- glm::mat4x4 GameObject::GenerateNormalTransform();
- void GameObject::UpdateModel(int modify);
- void GameObject::Update();

***************************************************************************/


#include <iostream>
#include <GLM/gtc/constants.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "GameObject.h"

/**************************************************************************
*!
\fn     GameObject::GameObject

\brief 
The default contructor for the class

\param  glm::vec3 position
the position of the objects

\param  glm::vec3 scale
the scale of the object

\param  const char* name
the name for the object

\param  Model::Shape shape
						
the model shape it wil have

\param  std::string textureFile
the address of the texture we want the object to have

\param  bool active
if is active or not

*
**************************************************************************/
GameObject::GameObject(glm::vec3 position, glm::vec3 scale, const char* name, Model::Shape shape, 
						std::string normalMap, std::string textureFile,  bool active, glm::vec3 up, glm::vec3 forward) : mMaterial(textureFile, normalMap), mModel(shape)
{
#pragma region INITIALIZING THE VARIABLES
	mPosition = position;
	mScale = scale;
	mName = name;

	mForward = forward;
	mUp = up;
	mRight = glm::cross(mUp, mForward);

	mActive = active;
#pragma endregion

}


/**************************************************************************
*!
\fn     GameObject::GenerateM2W

\brief 
Creates the model to world matrix

\return glm::mat4x4
returns the matrix

*
**************************************************************************/
glm::mat4x4 GameObject::GenerateM2W()
{
#pragma region CREATING THE NECESSARY MATRICES
	glm::mat4x4 m2w(1.0);
	m2w = glm::translate(m2w, mPosition);
	m2w = m2w * GenerateOrthogonal();
	mModel2World = glm::scale(m2w, mScale);
	
#pragma endregion

	//returning the matrix
	return mModel2World;
}

/**************************************************************************
*!
\fn     GameObject::GenerateOrthogonal

\brief 
Creates the orthgonal methop matrix

\return glm::mat4x4
returns the matrix

*
**************************************************************************/
glm::mat4x4 GameObject::GenerateOrthogonal()
{
	//Creating the matrix
	mOrthogonal = glm::mat4x4(

		mRight.x, mRight.y, mRight.z, 0,
		mUp.x,      mUp.y,      mUp.z,      0,
	    mForward.x,  mForward.y, mForward.z,   0,
		0, 0, 0, 1
	);

	//returning the matrix
	return mOrthogonal;
}

/**************************************************************************
*!
\fn     GameObject::GenerateAxisAngleMetod

\brief 
Creates the axis angle method matrix

\param  Axis rotationAxis
the axis around we want to rotate around

\param  float amount
the amout of degrees we want to rotate

\return glm::mat4x4
returns the matrix

*
**************************************************************************/
glm::mat4x4 GameObject::GenerateAxisAngleMetod(Axis rotationAxis, float amount)
{
	
	//if we are rotating around X axis of the object
	if (rotationAxis == Axis::X)
	{
		//computing the angle of rotation
		float angle = glm::pi<float>() / 180 * amount;

		//doing the tensor product with the normal vector in this case the right
		glm::mat4x4 tensor_normal
		(
			(mRight.x * mRight.x), (mRight.y * mRight.x), (mRight.x * mRight.z), 0.0F,
			(mRight.x * mRight.y), (mRight.y * mRight.y), (mRight.y * mRight.z), 0.0F,
			(mRight.x * mRight.z), (mRight.y * mRight.z), (mRight.z * mRight.z), 0.0F,
			0, 0, 0, 1.0F
		);

		//computing the needed data fot the multiplications
		float cosine = cosf(angle);

		glm::mat4x4 rot(1.0F);
		rot * cosine;
		rot[3][3] = 1;

		float vue = 1.0F - cosine;

		glm::mat4x4 temp_matrix = tensor_normal * vue;

		glm::mat4x4 n_tilda(
			0.0f, -mRight.z, mRight.y, 0.0F,
			mRight.z, 0.0F, -mRight.x, 0.0F,
			-mRight.y, mRight.x, 0.0F, 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F

		);

		glm::mat4x4 sine_times_n = n_tilda * sinf(angle);

		//doing the final matrix multiplication to stack them in one
		mAxisAngleMethod = rot + temp_matrix + sine_times_n;
	}

	//if we are rotating around Y axis of the object
	if (rotationAxis == Axis::Y)
	{
		//computing the angle of rotation
		float angle = glm::pi<float>() / 180 * amount;

		//doing the tensor product with the normal vector in this case the up
		glm::mat4x4 tensor_normal
		(
			(mUp.x * mUp.x), (mUp.y * mUp.x), (mUp.x * mUp.z), 0.0F,
			(mUp.x * mUp.y), (mUp.y * mUp.y), (mUp.y * mUp.z), 0.0F,
			(mUp.x * mUp.z), (mUp.y * mUp.z), (mUp.z * mUp.z), 0.0F,
			0, 0, 0, 1.0F
		);

		//computing the needed data fot the multiplications
		float cosine = cosf(angle);

		glm::mat4x4 rot(1.0F);
		rot * cosine;
		rot[3][3] = 1.0F;

		float vue = 1.0F - cosine;

		glm::mat4x4 temp_matrix = vue * tensor_normal;

		glm::mat4x4 n_tilda(
			0.0f, -mUp.z, mUp.y, 0.0F,
			mUp.z, 0.0F, -mUp.x, 0.0F,
			-mUp.y, mUp.x, 0.0F, 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F

		);

		glm::mat4x4 sine_times_n = sinf(angle) * n_tilda;

		//doing the final matrix multiplication to stack them in one
		mAxisAngleMethod = sine_times_n + temp_matrix + rot;
	}

	//if we are rotating around Z axis of the object
	if (rotationAxis == Axis::Z)
	{
		//computing the angle of rotation
		float angle = glm::pi<float>() / 180 * amount;

		//doing the tensor product with the normal vector in this case the forward
		glm::mat4x4 tensor_normal
		(
			(mForward.x * mForward.x), (mForward.y * mForward.x), (mForward.x * mForward.z), 0.0F,
			(mForward.x * mForward.y), (mForward.y * mForward.y), (mForward.y * mForward.z), 0.0F,
			(mForward.x * mForward.z), (mForward.y * mForward.z), (mForward.z * mForward.z), 0.0F,
			0, 0, 0, 1.0F
		);

		//computing the needed data fot the multiplications
		float cosine = cosf(angle);

		glm::mat4x4 rot(1.0F);
		rot * cosine;

		float vue = 1.0F - cosine;

		glm::mat4x4 temp_matrix = vue * tensor_normal;

		glm::mat4x4 n_tilda(
			0.0f, -mForward.z, mForward.y, 0.0F,
			mForward.z, 0.0F, -mForward.x, 0.0F,
			-mForward.y, mForward.x, 0.0F, 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F

		);

		glm::mat4x4 sine_times_n = n_tilda * sinf(angle);

		//doing the final matrix multiplication to stack them in one
		mAxisAngleMethod = sine_times_n + temp_matrix + rot;
	}

	//returning the matrix
	return mAxisAngleMethod;
}

/**************************************************************************
*!
\fn     GameObject::UpdateModel

\brief 
Updates the model based on the modification is given

\param  int modify
the modification we want to do to de model

*
**************************************************************************/
void GameObject::UpdateModel(int modify)
{
	//calling to the update function of te model
	mModel.UpdateModel(modify);
}

/**************************************************************************
*!
\fn     GameObject::Update

\brief 
The update function for the object

*
**************************************************************************/
void GameObject::Update()
{
	//creating a new model to world matrix
	GenerateM2W();
}


