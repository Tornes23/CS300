/*!**************************************************************************
\file    GameObject.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 06:57:10 2020

\brief   This file contains the
definition for the game object class

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
#pragma once

#include <GLM/mat4x4.hpp>
#include <GLM/vec4.hpp>
#include "../Model/Model.h"
#include "../Material/Material.h"
#include <vector>

struct GameObject
{
	enum Axis {
		X,
		Y,
		Z
	};

	GameObject(glm::vec3 position = glm::vec3(0.0F,0.0F,-100.0F), glm::vec3 scale = glm::vec3(50.0F,50.0F,50.0F), 
				const char* name = "Box", Model::Shape shape = Model::Shape::Cube, std::string normalMap = "./src/Texture/resources/normal_map.png", 
				std::string textureFile = "./src/Texture/resources/default.png" , bool active = false, glm::vec3 up = glm::vec3(0,1,0), glm::vec3 forward = glm::vec3(0, 0, 1));
	
	//member functions
	glm::mat4x4 GenerateM2W();
	glm::mat4x4 GenerateOrthogonal();
	glm::mat4x4 GenerateAxisAngleMetod(Axis rotationAxis, float amount);
	void UpdateModel(int modify);
	void Update();
	void Edit(int id);

	//member variables
	glm::mat4x4 mModel2World;
	glm::mat4x4 mOrthogonal;
	glm::mat4x4 mAxisAngleMethod;

	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mRight;

	Material mMaterial;
	Model mModel;

	std::string mName;
	bool mActive;

};
