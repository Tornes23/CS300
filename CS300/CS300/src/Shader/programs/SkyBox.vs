#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;

out vec3 UV;

//uniform variables for the transformation
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //computing the model to projection matrix 
    mat4 MV = projection * view;
    
    //applying the transformation to the vertex pos
    vec4 pos = MV * vec4(vPos, 1.0);
    
    gl_Position = pos.xyww;
    
    //setting the out variable
    UV = vPos;
}  