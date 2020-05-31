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
	ShaderProgram(std::string vertex, std::string fragment);
	const GLuint GetHandle() const;

	const GLuint GetUniformLoc(const std::string& name) const;

	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, float* values);

	void Use();


private:
	Shader mFragment;
	Shader mVertex;
	GLuint mHandle;
};


