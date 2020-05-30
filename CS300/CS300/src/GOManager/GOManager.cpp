/*!**************************************************************************
\file    GOManager.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 07:02:20 2020

\brief   This file contains the 
implementation of the GameObjectManager class

The functions included are:
- void GameObjectManager::AddObject(GameObject* obj);
- GameObject* GameObjectManager::CreateObject(glm::vec3 position, glm::vec3 scale, const char * name, Model::Shape shape, bool active, std::string textureName);
- void GameObjectManager::Update();
- GameObject * GameObjectManager::FindObject(const char * name);
- GameObject * GameObjectManager::FindActiveObject();
- GameObjectManager::~GameObjectManager();

***************************************************************************/


#include "GOManager.h"
#include "../Input/Input.h"

/**************************************************************************
*!
\fn     GameObjectManager::AddObject

\brief 
Adds an object to the container

\param  GameObject* obj
 pointer to the object to be added

*
**************************************************************************/
void GameObjectManager::AddObject(GameObject* obj)
{
	//adding the object to the container
	mObjects.push_back(obj);
}

/**************************************************************************
*!
\fn     GameObjectManager::CreateObject

\brief 
Creates a Game Object

\param  glm::vec3 position


\param  glm::vec3 scale


\param  const char * name


\param  Model::Shape shape


\param  bool active


\param  std::string textureName


\return GameObject*
Returns the game Object

*
**************************************************************************/
GameObject* GameObjectManager::CreateObject(glm::vec3 position, glm::vec3 scale, const char * name, Model::Shape shape, bool active, std::string textureName)
{
	//creating a object with the values and returning it
	return new GameObject(position, scale, name, shape, textureName, active);
}

/**************************************************************************
*!
\fn     GameObjectManager::Update

\brief 
The update function for the manager

*
**************************************************************************/
void GameObjectManager::Update()
{
	//getting the active object
	GameObject* obj = FindActiveObject();
	glm::vec4 temp;
	glm::mat4x4 axis_angle;

#pragma region ADDITION OF SLICES
	if (KeyDown(Plus))
		obj->UpdateModel(1);

	if (KeyDown(Minus))
		obj->UpdateModel(-1);
#pragma endregion

#pragma region GAME OBJECT ROTATIONS

#pragma region ROTATION AROUND X
	if (KeyDown(Q))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::X, 1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mUp, 1.0F);

		obj->mUp = axis_angle * temp;
		obj->mUp = glm::normalize(obj->mUp);
	}

	if (KeyDown(E))
	{

		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::X, -1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mUp, 1.0F);

		obj->mUp = axis_angle * temp;
		obj->mUp = glm::normalize(obj->mUp);
	}
#pragma endregion

#pragma region ROTATION AROUND Y
	if (KeyDown(Left))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::Z, 1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mRight, 1.0F);

		obj->mRight = axis_angle * temp;
		obj->mRight = glm::normalize(obj->mRight);
	}

	if (KeyDown(Right))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::Z, -1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mRight, 1.0F);

		obj->mRight = axis_angle * temp;
		obj->mRight = glm::normalize(obj->mRight);

	}
#pragma endregion

#pragma region ROTATION AROUND Z
	if (KeyDown(Up))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::Y, 1.0F);

		temp = glm::vec4(obj->mUp, 1.0F);

		obj->mUp = axis_angle * temp;
		obj->mUp = glm::normalize(obj->mUp);

		temp = glm::vec4(obj->mRight, 1.0F);

		obj->mRight = axis_angle * temp;
		obj->mRight = glm::normalize(obj->mRight);
	}

	if (KeyDown(Down))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::Y, -1.0F);

		temp = glm::vec4(obj->mUp, 1.0F);

		obj->mUp = axis_angle * temp;
		obj->mUp = glm::normalize(obj->mUp);

		temp = glm::vec4(obj->mRight, 1.0F);

		obj->mRight = axis_angle * temp;
		obj->mRight = glm::normalize(obj->mRight);
	}

#pragma endregion

#pragma endregion

#pragma region CHANGING ACTIVE OBJECT

	if (KeyTriggered(num1))
	{
		obj->mActive = false;
		FindObject("Plane")->mActive = true;
	}

	if (KeyTriggered(num2))
	{
		obj->mActive = false;
		FindObject("Cube")->mActive = true;
	}

	if (KeyTriggered(num3))
	{
		obj->mActive = false;
		FindObject("Cone")->mActive = true;
	}

	if (KeyTriggered(num4))
	{
		obj->mActive = false;
		FindObject("Cylinder")->mActive = true;
	}

	if (KeyTriggered(num5))
	{
		obj->mActive = false;
		FindObject("Sphere")->mActive = true;
	}

#pragma endregion

	//calling update for each object
	for (unsigned i = 0; i < mObjects.size(); i++)
	{
		mObjects[i]->Update();
	}
}

/**************************************************************************
*!
\fn     GameObjectManager::FindObject

\brief 
Finds a object given the name of it

\param  const char * name
The name of the object

\return GameObject *
returns a pointer to the object or null if was not found

*
**************************************************************************/
GameObject * GameObjectManager::FindObject(const char * name)
{
	//looping throught the vector
	for (auto it = mObjects.begin(); it != mObjects.end(); it++)
	{
		//if is the one we want
		if ((*it)->mName == name)
			return (*it);//returning the object
	}

	//if the find fails returning null
	return nullptr;
}

/**************************************************************************
*!
\fn     GameObjectManager::FindActiveObject

\brief 
Finds the first active object

\return GameObject *


*
**************************************************************************/
GameObject * GameObjectManager::FindActiveObject()
{
	//looping throught the vector
	for (auto it = mObjects.begin(); it != mObjects.end(); it++)
	{
		//if is the one we want
		if ((*it)->mActive)
			return (*it);//returning the object
	}

	//if the find fails returning null
	return nullptr;
}

/**************************************************************************
*!
\fn     GameObjectManager::~GameObjectManager

\brief 
The destructor of the class

*
**************************************************************************/
GameObjectManager::~GameObjectManager()
{
	//freeing the memory
	for (auto it = mObjects.begin(); it != mObjects.end(); it++)
	{
		(*it)->mModel.FreeBuffers();
		delete *it;
	}

	//clearing the vector
	mObjects.clear();
}


