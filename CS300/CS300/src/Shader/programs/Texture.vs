#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;

//out variables for the fragment shader
out vec2 UV;

//uniform variables for the transformation
uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 m2w_normal;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    //setting the out variable
    UV = vec2(vTextCoords.x, 1 - vTextCoords.y);
}  