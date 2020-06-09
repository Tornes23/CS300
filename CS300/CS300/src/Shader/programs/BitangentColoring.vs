#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 2) in vec3 vNormal;
layout(location = 4) in vec3 vTangent;

//out variables for the fragment shader
out vec3 BitTangent;

//uniform variables for the transformation
uniform mat4 m2w_normal;
uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
    mat4 MV = view * m2w;
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    vec3 Normal = vec3(normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)));
    
    vec3 Tangent = vec3(normalize(vec4(mat3(m2w_normal) * vTangent, 0.0)));
    
    //Computing the tangent to render
    BitTangent = normalize(cross(Normal, Tangent));
    
} 