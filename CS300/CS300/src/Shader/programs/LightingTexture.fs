#version 400 core

struct Light {
    vec3 Position;
    vec3 Direction;
  
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
};

uniform Light light; 

struct Material{
    vec3 DiffuseColor;
    vec3 SpecularColor;
    
    float Sininess;
    float AmbientCefficient;
};

uniform Material material;

out vec4 FragColor;  

in vec2 UV;

uniform sampler2D textureData;
 
void main()
{
    FragColor = texture(textureData, UV).rgba;
}