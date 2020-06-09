#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 4) in vec3 vTangent;

//out variables for the fragment shader
out vec3 Tangent;

//uniform variables for the transformation
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
    
    //Computing the tangent to render
    Tangent = vec3(MV * vec4(vTangent, 0.0));
    
} 