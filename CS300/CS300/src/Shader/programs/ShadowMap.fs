#version 400 core

uniform sampler2D textureData;

in vec2 UV;

out vec3 fragColor;

uniform float Contrast;

void main()
{
//computing the color based on the contrast value
    float color = texture(textureData, UV).r;

    
    color = (color - (1.0 - Contrast)) / Contrast;
    
    fragColor = vec3(color); 
}