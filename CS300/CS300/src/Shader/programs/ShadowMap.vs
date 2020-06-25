
#version 400 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vUV;

out vec2 UV;

uniform mat4 M;


void main()
{
	UV = vUV;
    
	gl_Position =  M * vec4(vPosition, 1);
}
