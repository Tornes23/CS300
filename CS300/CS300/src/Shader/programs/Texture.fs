#version 400 core

out vec4 FragColor;  

in vec2 UV;
in vec3 Normal;

uniform sampler2D textureData;
 
void main()
{
    FragColor = texture(textureData, UV).rgba;
}