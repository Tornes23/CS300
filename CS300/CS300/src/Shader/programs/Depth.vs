#version 400

//variable in the VBO
layout(location = 0) in vec3 vPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 m2w;

void main()
{
    
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
   //setting the position
   gl_Position = MVP * vec4(vPos, 1.0F);
}