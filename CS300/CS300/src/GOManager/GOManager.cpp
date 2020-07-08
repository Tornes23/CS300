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
- void GameObjectManager::EditObj(GameObject* obj);
- GameObject * GameObjectManager::FindObject(const char * name);
- GameObject * GameObjectManager::FindActiveObject();
- GameObjectManager::~GameObjectManager();

***************************************************************************/


#include "GOManager.h"
#include "../Input/Input.h"
#include <GLM/gtc/type_ptr.hpp>
#include <IMGUI/imgui.h>

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
	mAllObjects.push_back(obj);
}

void GameObjectManager::AddLevelObject(GameObject * obj)
{
	mAllObjects.push_back(obj);
	mLevelObjects.push_back(obj);
}

/**************************************************************************
*!
\fn     GameObjectManager::CreateObject

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

\param  std::string normalMap
the address of the normal map we want the object to have

\param  std::string textureFile
the address of the texture we want the object to have

\param  bool active
if is active or not

*
**************************************************************************/
GameObject* GameObjectManager::CreateObject(glm::vec3 position, glm::vec3 scale, const char * name, Model::Shape shape, bool active, glm::vec3 up, glm::vec3 forward, std::string normalMap, std::string textureName)
{
	//creating a object with the values and returning it
	return new GameObject(position, scale, name, shape, normalMap, textureName, active, up, forward);
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
	GameObject* side1 = FindObject("SideKickLeft");
	GameObject* side2 = FindObject("SideKickRight");

	glm::vec4 temp;
	glm::mat4x4 axis_angle;

#pragma region ADDITION OF SLICES
	if (KeyDown(Plus))
		obj->UpdateModel(1);

	if (KeyDown(Minus))
		obj->UpdateModel(-1);
#pragma endregion

#pragma region GAME OBJECT ROTATIONS

#pragma region ROTATION AROUND Y
	if (KeyDown(Left))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::Y, 1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mRight, 1.0F);

		obj->mRight = axis_angle * temp;
		obj->mRight = glm::normalize(obj->mRight);
	}

	if (KeyDown(Right))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::Y, -1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mRight, 1.0F);

		obj->mRight = axis_angle * temp;
		obj->mRight = glm::normalize(obj->mRight);

	}
#pragma endregion

#pragma region ROTATION AROUND X
	if (KeyDown(Up))
	{
		axis_angle = obj->GenerateAxisAngleMetod(GameObject::Axis::X, 1.0F);

		temp = glm::vec4(obj->mForward, 1.0F);

		obj->mForward = axis_angle * temp;
		obj->mForward = glm::normalize(obj->mForward);

		temp = glm::vec4(obj->mUp, 1.0F);

		obj->mUp = axis_angle * temp;
		obj->mUp = glm::normalize(obj->mUp);
	}

	if (KeyDown(Down))
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

#pragma endregion

#pragma region CHANGING ACTIVE OBJECT

	if (KeyTriggered(num1))
	{
		obj->mActive = false;
		FindObject("Plane")->mActive = true;

		side1->mModel.SetShape(Model::Shape::Plane);
		side2->mModel.SetShape(Model::Shape::Plane);
	}

	if (KeyTriggered(num2))
	{
		obj->mActive = false;
		FindObject("Cube")->mActive = true;

		side1->mModel.SetShape(Model::Shape::Cube);
		side2->mModel.SetShape(Model::Shape::Cube);
	}

	if (KeyTriggered(num3))
	{
		obj->mActive = false;
		FindObject("Cone")->mActive = true;

		side1->mModel.SetShape(Model::Shape::Cone);
		side2->mModel.SetShape(Model::Shape::Cone);
	}

	if (KeyTriggered(num4))
	{
		obj->mActive = false;
		FindObject("Cylinder")->mActive = true;

		side1->mModel.SetShape(Model::Shape::Cylinder);
		side2->mModel.SetShape(Model::Shape::Cylinder);
	}

	if (KeyTriggered(num5))
	{
		obj->mActive = false;
		FindObject("Sphere")->mActive = true;

		side1->mModel.SetShape(Model::Shape::Sphere);
		side2->mModel.SetShape(Model::Shape::Sphere);
	}

	if (KeyTriggered(O))
		mbSideAnim = !mbSideAnim;

#pragma endregion

	Edit();

	//calling update for each object
	for (unsigned i = 0; i < mObjects.size(); i++)
		mObjects[i]->Update();

	if (mbSideAnim)
	{
		mSidePos += 1.0F;
		UpdateSideKicks(side1, side2);
	}
}

void GameObjectManager::UpdateSideKicks(GameObject * side1, GameObject * side2)
{
	if (mSidePos >= 360.0F)
		mSidePos = 0.0F;

	if (side1 != nullptr)
	{
		//computing the position and setting it
		float posX = -20.0F * cosf(glm::radians(mSidePos));
		float posY = (-20.0F / 2.0F) * sinf(glm::radians(mSidePos) * 5);
		float posZ = -20.0F * sinf(glm::radians(mSidePos));

		side1->mPosition = glm::vec3(posX, posY, posZ);
	}

	if (side2 != nullptr)
	{
		//computing the position and setting it
		float posX = 20.0F * cosf(glm::radians(mSidePos));
		float posY = (20.0F / 2.0F) * sinf(glm::radians(mSidePos) * 5);
		float posZ = 20.0F * sinf(glm::radians(mSidePos));

		side2->mPosition = glm::vec3(posX, posY, posZ);
	}
}

/**************************************************************************
*!
\fn     GameObjectManager::EditObj

\brief
Finds a object given the name of it

\param GameObject *
The object to be edited

*
**************************************************************************/
void GameObjectManager::Edit()
{
	//creating the window
	if (!ImGui::Begin("Game Objects"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	for (unsigned i = 0; i < mAllObjects.size(); i++)
	{
		//if not active
		if (!mAllObjects[i]->mActive)
			continue;

		//setting the tab title
		std::string title = mAllObjects[i]->mName;

		//calling to the edit of the object
		if (ImGui::CollapsingHeader(title.c_str()))
		{
			mAllObjects[i]->Edit(i);
		}
	}
	//to edit the transform
	
	
	ImGui::End();
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
	for (auto it = mAllObjects.begin(); it != mAllObjects.end(); it++)
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
	for (auto it = mAllObjects.begin(); it != mAllObjects.end(); it++)
		(*it)->mModel.FreeBuffers();

	//clearing the vector
	mObjects.clear();
	mLevelObjects.clear();
	mAllObjects.clear();
}

GameObjectManager::GameObjectManager()
{
	mSidePos = 0.0F;
	mbSideAnim = true;
}


