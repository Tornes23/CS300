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

#pragma once
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GLM/vec3.hpp>
#include <GLM/vec2.hpp>
#include "../Utilities/Utilities.h"

class Model
{
public:

	enum Shape {
		Plane,
		Cube,
		Cone,
		Cylinder,
		Sphere
	};

	Model(Shape form = Cube, int precision = 9);

	void UpdateModel(int modification);
	void CreateSphere(int slices = 9, float radius = 1.0F);
	void CreateCylinder(int slices = 9, float radius = 1.0F, float height = 1.0F);
	void CreateCone(int slices = 9, float radius = 1.0F, float height = 1.0F);
	void CreateCube();
	void CreatePlane();

	void BindBuffers();
	void BindNormalBuffer();
	void BindModelBuffer();
	void FreeBuffers();
	void GenBuffers();

	GLsizei GetDrawElements() const;
	bool GetIndexed();

	const GLuint GetVAO() const;
	const GLuint GetNormalVAO() const;
	Shape GetShape() const;
	void SetShape(Shape shape);

private:

	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec2> mTextureCoords;
	std::vector<glm::vec3> mNormalVecs;
	std::vector<glm::vec3> mAveraged;
	std::vector<unsigned short> mIndexes;
	

	bool mIndexed;
	int mPrecision;
	Shape mShape;
	GLuint mVAO[3];
	GLuint mVBO[5];
};