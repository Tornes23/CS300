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
	void BindModelBuffer();
	void BindNormalBuffer();
	void FreeBuffers();
	void GenBuffers();

	void GenNormals();
	void GenTangents();
	void GenAvgTangents();

	void SortAvgTangents();
	void SortAvgBitangents();

	glm::vec3 AddTangents(int index);
	glm::vec3 AddBitangents(int index);

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
	std::vector<glm::vec3> mTangents;
	std::vector<glm::vec3> mAvgTangents;
	std::vector<glm::vec3> mBitangents;
	std::vector<glm::vec3> mAvgBitangents;
	std::vector<unsigned short> mIndexes;
	
	std::vector<std::vector<glm::vec3>> mTangetsPerVertex;
	std::vector<std::vector<glm::vec3>> mBitangetsPerVertex;

	bool mIndexed;
	int mPrecision;
	Shape mShape;
	GLuint mVAO[2];
	GLuint mVBO[9];
};