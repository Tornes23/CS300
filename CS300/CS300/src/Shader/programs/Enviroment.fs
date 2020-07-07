
#version 400 core

out vec3 color;

in vec3 fragPos;

uniform samplerCube cubemap;

void main()
{
    color = texture(cubemap, normalize(fragPos)).rgb; 
}
