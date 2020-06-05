#version 400 core

struct Light{
    
    int Type;

    vec3 Position;
    vec3 Direction;
    vec3 PosInCamSpc;
    
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    vec3 Attenuation;
};


struct Material{

    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    
    float Sininess;
    
};

uniform Light lightSource; 
uniform Material material;

out vec3 FragColor; 
 
in vec3 PosInCamSpc; 
in vec2 UV;
in vec3 Normal;

uniform sampler2D textureData;

vec3 PointLight(vec3 diffuseCol)
{
    vec3 lightDir = normalize(lightSource.PosInCamSpc - PosInCamSpc);
    vec3 reflection = reflect(-lightDir, Normal);
    
    diffuseCol += max(dot(Normal, lightDir), 0.0) * lightSource.DiffuseColor * material.DiffuseColor 
                + max(pow(dot(normalize(-PosInCamSpc), reflection), material.Sininess), 0.0) * lightSource.SpecularColor * material.SpecularColor;
                
    return diffuseCol;
}

//vec3 DirectionalLight(vec3 diffuseCol)
//{
//    
//}
//
//vec3 SpotLight(vec3 diffuseCol)
//{
//    
//}

vec3 ApplyPhongLight()
{
    vec3  finalCol;
    float attenuation;
    float SpotLight = 1.0;
    float distance = length(lightSource.PosInCamSpc - PosInCamSpc);
    vec3  diffuse = texture(textureData, UV).rgb;
    vec3  ambientCol = lightSource.AmbientColor * material.AmbientColor * diffuse;
    
    if(lightSource.Type == 0)
    {
        vec3 color = PointLight(diffuse);
        
        attenuation = min(1.0 / (lightSource.Attenuation.x + lightSource.Attenuation.y * distance + lightSource.Attenuation.z * distance * distance), 1.0);  
        
        finalCol = color * attenuation * clamp(SpotLight, 0.0F, 1.0F);
        
        return finalCol;
    }
    
    if(lightSource.Type == 1)
    {
        //DirectionalLight();
    }
    
    if(lightSource.Type == 2)
    {
        //SpotLight();
    }
}

void main()
{   
    FragColor = ApplyPhongLight();
    
}