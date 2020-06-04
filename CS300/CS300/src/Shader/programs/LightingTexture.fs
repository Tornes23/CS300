#version 400 core

struct Light{
    vec3 Position;
    vec3 Direction;
    
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    
    float Constant;
    float Linear;
    float Quadratic;
};


struct Material{

    vec3 DiffuseColor;
    vec3 SpecularColor;
    
    float Sininess;
    float AmbientCoefficient;
    
};

uniform Light lightSource; 
uniform Material material;

out vec4 FragColor; 

in vec3 Pos; 
in vec2 UV;

uniform sampler2D textureData;
 
void main()
{
    // ambient
    vec3 ambient = (material.AmbientCoefficient * vec3(lightSource.Ambient)) * texture(textureData, UV).rgb;
  	
    vec3 result = material.AmbientCoefficient * ambient;
    
    FragColor = vec4(ambient, 1.0);
    
}