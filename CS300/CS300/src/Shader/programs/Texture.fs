#version 400 core

//output fragmet color
out vec4 FragColor;  

//in variables that we get from the vertex shader
in vec2 UV;
in vec3 Normal;

//uniform variable to get the texture data
uniform sampler2D textureData;
 
void main()
{
    //getting and setting the color of the pixel from the texture
    FragColor = texture(textureData, UV).rgba;
}