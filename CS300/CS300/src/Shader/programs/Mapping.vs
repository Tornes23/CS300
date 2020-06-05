#version 400 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;

out vec2 UV;

uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 MVP = projection * view * m2w;
    
    gl_Position = MVP * vec4(vPos, 1.0);
    
    UV = vec2(vTextCoords.x, vTextCoords.y);
}  