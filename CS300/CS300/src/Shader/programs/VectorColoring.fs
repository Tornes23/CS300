#version 400 core

//output fragmet color
out vec4 FragColor;  

in vec3 Vector;
 
void main()
{
    //getting and setting the color of the pixel from the texture
    FragColor = vec4(Vector, 1.0F);
}