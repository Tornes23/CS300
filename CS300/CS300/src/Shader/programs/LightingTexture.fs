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
    
    float Shininess;
    
};

uniform Light lightSources[8];
uniform int lightCount; 
uniform Material material;

out vec4 FragColor; 
 
in vec3 PosInCamSpc; 
in vec2 UV;
in vec3 Normal;

uniform sampler2D textureData;

vec3 PointLight(vec3 initialCol, int i)
{
    vec3 lightDir = normalize(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing diffuse color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    initialCol += diffuseCol;
    
    //computing specular color
    
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    initialCol += specular;
    
    return initialCol;
}

vec3 DirectionalLight(vec3 initialCol, int i)
{
    vec3 lightDir = normalize(-lightSources[i].Direction);
    
    //computing diffuse color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    initialCol += diffuseCol;
    
    //computing specular color
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin
    
    vec3 reflectDir = reflect(lightDir, Normal);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    initialCol += specular;
    
    return initialCol; 
}

vec3 SpotLight(vec3 initialCol, int i)
{
     vec3 lightDir = normalize(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing diffuse color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    initialCol += diffuseCol;
    
    //computing specular color
    
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    initialCol += specular;
    
    //vec3 spotCol = ;
    
    float distance = length(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    float attenuation = min((1.0 / 
                        (lightSources[i].Attenuation.x + (lightSources[i].Attenuation.y * distance) 
                        + lightSources[i].Attenuation.z * (distance * distance))), 1.0);
    
    initialCol *= attenuation;
    
    return initialCol;
}

vec3 ApplyPhongLight()
{
    vec3  finalCol;
    float attenuation;
    float SpotLight = 1.0;
    
    for(int i = 0; i < lightCount; i++)
    {
        vec3  textureCol = texture(textureData, UV).rgb;
        vec3  ambientCol = lightSources[i].AmbientColor * material.AmbientColor;
        vec3 color;
        float attenuation;
        float distance = length(lightSources[i].PosInCamSpc - PosInCamSpc);
        
        if(lightSources[i].Type == 0)
        {
            color = PointLight(ambientCol, i);
            
            //finalCol = color * textureCol;
    
            attenuation = min((1.0 / 
                        (lightSources[i].Attenuation.x + (lightSources[i].Attenuation.y * distance) 
                        + lightSources[i].Attenuation.z * (distance * distance))), 1.0);
    
            
            //return finalCol;
        }
        
        if(lightSources[i].Type == 1)
        {
            color = DirectionalLight(ambientCol, i);
            
            //finalCol = color * textureCol;
            
            attenuation = min((1.0 / 
                        (lightSources[i].Attenuation.x + (lightSources[i].Attenuation.y * distance) 
                        + lightSources[i].Attenuation.z * (distance * distance))), 1.0);
  
            
            //return finalCol;
            
        }
        
        if(lightSources[i].Type == 2)
        {
            //color = SpotLight(ambientCol);
            
            //finalCol = color * textureCol;
            //
            //return finalCol;
        }
        
        finalCol += color * attenuation * textureCol; //* clamp(SpotLight, 0.0, 1.0);;
    }
    
    return finalCol;
}

void main()
{   
    FragColor = vec4(ApplyPhongLight(), 1.0);
    
}