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
#ifdef USE_OPENGL
#include <GL/glew.h>
#endif
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

class Shader
{
public:
#ifdef USE_OPENGL
	Shader(GLenum shaderType, const char* filename);
	const GLuint GetHandle() const;
#endif
#ifdef USE_VULKAN
	Shader(): mFilename("Dummy.txt") {}
#endif

private:
	std::string mFilename;
#ifdef USE_OPENGL
	GLenum mType;
	GLuint mHandle;
#endif

#ifdef USE_VULKAN
	//culkan handles and other shit I may need
#endif
};

class ShaderProgram
{
public:
#ifdef USE_VULKAN
	ShaderProgram(){}
#endif

#ifdef USE_OPENGL
	ShaderProgram(std::string vertex, std::string fragment, std::string geometry = "");
	const GLuint GetHandle() const;

	const GLuint GetUniformLoc(const std::string& name) const;
#endif

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
#ifdef USE_OPENGL
	GLuint mHandle;
#endif

};


