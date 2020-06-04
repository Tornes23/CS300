#version 400 core

out vec4 FragColor;  

in vec2 UV;
flat in int AvNormal;

void main()
{
    

    if(AvNormal == 1)
        FragColor = vec4(1,0,0,1);
    else
        FragColor = vec4(1,1,1,1);
       
}