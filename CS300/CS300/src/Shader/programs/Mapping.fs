#version 400 core

//output fragmet color
out vec4 FragColor;  

//in variable to get from the vertex shader
in vec2 UV;
 
void main()
{
    //setting the output color
    FragColor = vec4(UV, 0, 1);
}