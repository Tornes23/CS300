#version 400 core

//variable in the VBO
layout(location = 0) in vec3 vPos;

//uniform variables for the transformation
uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
}  