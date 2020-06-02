#version 400 core

struct Light {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    FragColor = vec4(UV, 0, 1);
}