#version 400 core

//output fragmet color
out vec4 FragColor;  

//in variables that we get from the vertex shader
in vec3 UV;

//uniform variable to get the texture data
uniform samplerCube cubeMap;
 
void main()
{
    //getting and setting the color of the pixel from the texture
    FragColor = texture(cubeMap, UV);
        
}