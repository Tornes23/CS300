/*!**************************************************************************
\file    GOManager.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 07:02:20 2020

\brief   This file contains the
definition of the GameObjectManager class

The functions included are:
- void GameObjectManager::AddObject(GameObject* obj);
- GameObject* GameObjectManager::CreateObject(glm::vec3 position, glm::vec3 scale, const char * name, Model::Shape shape, bool active, std::string textureName);
- void GameObjectManager::Update();
- GameObject * GameObjectManager::FindObject(const char * name);
- GameObject * GameObjectManager::FindActiveObject();
- GameObjectManager::~GameObjectManager();

***************************************************************************/
#pragma once

#include "../GameObject/GameObject.h"

class GameObjectManager
{
	public:
		//object container
		std::vector<GameObject*> mObjects;
		std::vector<GameObject*> mLevelObjects;
		std::vector<GameObject*> mAllObjects;

		//utility functions
		void AddObject(GameObject* obj);
		void AddLevelObject(GameObject* obj);
		GameObject* CreateObject(glm::vec3 position, glm::vec3 scale, const char* name = "Box", Model::Shape shape = Model::Shape::Cube,
							     bool active = false, glm::vec3 up = glm::vec3(0,1,0), glm::vec3 forward = glm::vec3(0, 0, 1), std::string textureName = "./src/Texture/resources/default.png");
		void Update();
		void EditObj(GameObject* obj);
		GameObject* FindObject(const char* name = "Box");
		GameObject* FindActiveObject();
		~GameObjectManager();

};