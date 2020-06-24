#version 400 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUV;

uniform mat4 M;
out vec2 UV;

void main()
{
	gl_Position =  M * vec4(vPosition,1);
    
	UV = vUV;
}