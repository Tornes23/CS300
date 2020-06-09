#version 400 core

//output fragmet color
out vec4 FragColor;  

in vec3 Normal;
 
void main()
{
    //getting and setting the color of the pixel from the texture
    FragColor = vec4(Normal, 1.0F);
}