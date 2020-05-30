#version 400 core

in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 m2w;

void main()
{
    mat4 MVP = projection * view * m2w;
    
    gl_Position = MVP * vec4(aPos, 1.0);
}