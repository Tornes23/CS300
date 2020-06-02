#version 400 core

struct Light {
    vec3 Position;
    vec3 Direction;
  
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    
    float Constant;
    float Linear;
    float Quadratic;
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

in vec3 Pos; 
in vec2 UV;
in vec3 Normal;
uniform vec3 cameraPos;

uniform sampler2D textureData;
 
void main()
{
    // ambient
    vec3 ambient = vec3(light.Ambient) * texture(textureData, UV).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    
    vec3 lightDir = normalize(-light.Direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(light.Diffuse) * diff * texture(textureData, UV).rgb;  
    
    // specular
    vec3 viewDir = normalize(cameraPos - Pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Sininess);
    vec3 specular = vec3(light.Specular) * spec * texture(textureData, UV).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
    
    //FragColor = texture(textureData, UV).rgba;
}