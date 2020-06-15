#version 400 core

//output fragmet color
out vec4 FragColor;  

in vec3 lineColor;

void main()
{
    //setting the color as white
    FragColor = vec4(lineColor, 1);
}