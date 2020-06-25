#version 400 core

in vec2 UV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D textureData;

void main()
{	 
    FragColor =  texture(textureData, UV).rgba;
}