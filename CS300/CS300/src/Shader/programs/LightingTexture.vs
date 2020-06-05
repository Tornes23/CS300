#version 400 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAverage;

out vec2 UV;
out vec3 Normal;

uniform int Average;
uniform mat4 m2w;
uniform mat4 m2w_normal;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 MVP = projection * view * m2w;
    
    gl_Position = MVP * vec4(vPos, 1.0);
    
    UV = vec2(vTextCoords.x, 1 - vTextCoords.y);
    
    if(Average == 1)
        Normal = vec3(normalize(vec4(mat3(m2w_normal) * vAverage, 0.0)));
    else
        Normal = vec3(normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)));
    
}  