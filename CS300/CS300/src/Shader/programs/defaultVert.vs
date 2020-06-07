#version 400

//variable in the VBO
layout(location = 0) in vec4 position;

void main()
{
   //setting the position
   gl_Position = position;
}