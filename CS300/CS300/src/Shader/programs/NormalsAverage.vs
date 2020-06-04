#version 400 core

layout(location = 0) in vec3 vPos;
layout(location = 2) in vec3 vAverage;

out VS_OUT {
    vec4 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 m2w;
uniform mat4 m2w_normal;

void main()
{
    mat4 MVP = projection * view * m2w;
     
    gl_Position = MVP * vec4(vPos, 1.0); 
    
    vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vAverage, 0.0));
        
}