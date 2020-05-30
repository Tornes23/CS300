/*!**************************************************************************
\file    Shader.cpp

\author  Nestor Uriarte 

\par     DP email:  nestor.uriarte@digipen.edu 

\par     Course:    CS300 

\par     assignemnt 0 

\date    Mon May 25 07:15:40 2020

\brief   This file contains the 
implementation of the ShaderProgram and Shader classes

The functions included are:
- ShaderProgram::ShaderProgram(std::string vertex, std::string fragment);
- const GLuint ShaderProgram::GetHandle() const;
- const GLuint ShaderProgram::GetUniformLoc(const std::string& name) const;
- void ShaderProgram::SetUniform(const std::string & name, int value);
- void ShaderProgram::SetUniform(const std::string & name, float * values);
- Shader::Shader(GLenum shaderType, const char* filename);
- const GLuint Shader::GetHandle() const;

***************************************************************************/


#include <iostream>
#include "Shader.h"
#include "../Utilities/Utilities.h"

/**************************************************************************
*!
\fn     ShaderProgram::ShaderProgram

\brief 
The default constructor of the class

\param  std::string vertex
 a string containing the address of the vertex shader

\param  std::string fragment
a string containing the address of the fragment shader


*
**************************************************************************/
ShaderProgram::ShaderProgram(std::string vertex, std::string fragment) : mVertex(GL_VERTEX_SHADER, vertex.data()), mFragment(GL_FRAGMENT_SHADER, fragment.data())
{
	GLenum error = glGetError();
	//creating a shader program
	mHandle = glCreateProgram();
	error = glGetError();

	//attaching the fragment shader
	glAttachShader(mHandle, mFragment.GetHandle());
	error = glGetError();

	//attaching the vertex shader
	glAttachShader(mHandle, mVertex.GetHandle());
	error = glGetError();

	//linking both shaders
	glLinkProgram(mHandle);
	error = glGetError();

	//checking if it linked succesfully
	GLint status;
	glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(mHandle, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	//Detaching the fragment shader
	glDetachShader(mHandle, mFragment.GetHandle());
	error = glGetError();

	//Detaching the vertex shader
	glDetachShader(mHandle, mVertex.GetHandle());
	error = glGetError();

}

/**************************************************************************
*!
\fn     ShaderProgram::GetHandle

\brief 
Returns the handle of the shader program

\return const GLuint
returns the value of the handle

*
**************************************************************************/
const GLuint ShaderProgram::GetHandle() const
{
	//returning the handle
	return mHandle;
}

/**************************************************************************
*!
\fn     ShaderProgram::GetUniformLoc

\brief 
Gets the location of the given uniform variable

\param  const std::string& name
the name of the uniform variable

\return const GLuint
the handle of the location of the uniform

*
**************************************************************************/
const GLuint ShaderProgram::GetUniformLoc(const std::string& name) const
{
	//returning the location of the uniform
	return glGetUniformLocation(mHandle, name.c_str());
}

/**************************************************************************
*!
\fn     ShaderProgram::SetUniform

\brief 
Sets a uniform variable

\param  const std::string & name
the name of the uniform

\param  int value
the value to be set to the uniform

*
**************************************************************************/
void ShaderProgram::SetUniform(const std::string & name, int value)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cout << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniform1i(location, value);
}

/**************************************************************************
*!
\fn     ShaderProgram::SetUniform

\brief
Sets a uniform matrtix variable

\param  const std::string & name
the name of the uniform

\param  float * values
 array of floats to set to the uniform matrix

*
**************************************************************************/
void ShaderProgram::SetUniform(const std::string & name, float * values)
{
	//getting the location of the uniform
	GLuint location = GetUniformLoc(name);

	//if it was found
	if (location < 0)
	{
		std::cout << "Uniform: " << name << " not found" << std::endl;
		return;
	}

	//setting the value
	glUniformMatrix4fv(location, 1, GL_FALSE, values);
	
}

/**************************************************************************
*!
\fn     Shader::Shader

\brief 
The default constructor of the class

\param  GLenum shaderType
Enum that specifies which type of shader is

\param  const char* filename
The name of the file

*
**************************************************************************/
Shader::Shader(GLenum shaderType, const char* filename)
{
	//setting the member variables
	mType = shaderType;

	mFilename = filename;

	if (mFilename == "")
		return;

	//creating a shader with the given type
	mHandle = glCreateShader(mType);

	//string to store the parsed code
	std::string parsed;

	//calling to parse the code
	Utils::ParseShader(mFilename, parsed);

	//storing it in a const char * because of openGL
	const char* sourceCode = parsed.c_str();

	//setting the source code
	glShaderSource(mHandle, 1, &sourceCode, NULL);

	//compiling the shader code
	glCompileShader(mHandle);

	//checking if it compiled properly
	GLint status;
	glGetShaderiv(mHandle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;

		glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(mHandle, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (shaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

}

/**************************************************************************
*!
\fn     Shader::GetHandle

\brief 
Returns the handle of the shader

\return const GLuint
returns the handlke

*
**************************************************************************/
const GLuint Shader::GetHandle() const
{
	//returning the handle
	return mHandle;
}

