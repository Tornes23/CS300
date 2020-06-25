
#version 400 core

uniform sampler2D textureData;

in vec2 UV;

out vec3 color;

uniform float Contrast;

void main()
{
    float depth = texture(textureData, UV).r;

    depth = (depth - (1.0 - Contrast)) / Contrast;
    color = vec3(depth); 
}