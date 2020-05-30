#version 400 core

out vec4 FragColor;  

in vec2 UV;

uniform sampler2D textureData;
 
void main()
{
    FragColor = vec4(UV, 0, 1);
}