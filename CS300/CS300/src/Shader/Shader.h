/*!**************************************************************************
\file    Shader.cpp

\author  Nestor Uriarte

\par     DP email:  nestor.uriarte@digipen.edu

\par     Course:    CS300

\par     assignemnt 0

\date    Mon May 25 07:15:40 2020

\brief   This file contains the
definition of the ShaderProgram and Shader classes

The functions included are:
- ShaderProgram::ShaderProgram(std::string vertex, std::string fragment);
- const GLuint ShaderProgram::GetHandle() const;
- const GLuint ShaderProgram::GetUniformLoc(const std::string& name) const;
- void ShaderProgram::SetUniform(const std::string & name, int value);
- void ShaderProgram::SetUniform(const std::string & name, float * values);
- Shader::Shader(GLenum shaderType, const char* filename);
- const GLuint Shader::GetHandle() const;

***************************************************************************/
#pragma once
#include <string>
#include <GL/glew.h>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

class Shader
{
public:
	Shader(GLenum shaderType, const char* filename);
	const GLuint GetHandle() const;

private:
	std::string mFilename;
	GLenum mType;
	GLuint mHandle;
};

class ShaderProgram
{
public:
	ShaderProgram(std::string vertex, std::string fragment, std::string geometry = "");
	const GLuint GetHandle() const;

	const GLuint GetUniformLoc(const std::string& name) const;

	void SetIntUniform(const std::string& name, int value);
	void SetBoolUniform(const std::string& name, bool value);
	void SetFloatUniform(const std::string& name, float value);
	void SetMatUniform(const std::string& name, float* values);
	void SetVec3Uniform(const std::string& name, glm::vec3 values);
	void SetVec4Uniform(const std::string& name, glm::vec4 values);

	void Use();

private:
	Shader mFragment;
	Shader mVertex;
	Shader mGeometry;
	
	bool mbGeometry;
	GLuint mHandle;
};


