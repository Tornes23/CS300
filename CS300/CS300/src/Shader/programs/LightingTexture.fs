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

uniform Light lightSource; 
uniform Material material;

out vec4 FragColor; 
 
in vec3 PosInCamSpc; 
in vec2 UV;
in vec3 Normal;

uniform sampler2D textureData;

vec3 PointLight(vec3 initialCol)
{
    vec3 lightDir = normalize(lightSource.PosInCamSpc - PosInCamSpc);
    
    //computing diffuse color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSource.DiffuseColor;
    
    initialCol += diffuseCol;
    
    //computing specular color
    
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    
    vec3 specular = (material.SpecularColor * spec) * lightSource.SpecularColor;  
    
    initialCol += specular;
    
    float distance = length(lightSource.PosInCamSpc - PosInCamSpc);
    
    float attenuation = min((1.0 / 
                        (lightSource.Attenuation.x + (lightSource.Attenuation.y * distance) 
                        + lightSource.Attenuation.z * (distance * distance))), 1.0);
    
    initialCol *= attenuation;
    
    return initialCol;
}

vec3 DirectionalLight(vec3 initialCol)
{
    vec3 lightDir = normalize(-lightSource.Direction);
    
    //computing diffuse color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSource.DiffuseColor;
    
    initialCol += diffuseCol;
    
    //computing specular color
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin
    
    vec3 reflectDir = reflect(lightDir, Normal);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    
    vec3 specular = (material.SpecularColor * spec) * lightSource.SpecularColor;  
    
    initialCol += specular;
    
    float distance = length(lightSource.PosInCamSpc - PosInCamSpc);
    
    return initialCol; 
}

vec3 SpotLight(vec3 initialCol)
{
     vec3 lightDir = normalize(lightSource.PosInCamSpc - PosInCamSpc);
    
    //computing diffuse color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSource.DiffuseColor;
    
    initialCol += diffuseCol;
    
    //computing specular color
    
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    
    vec3 specular = (material.SpecularColor * spec) * lightSource.SpecularColor;  
    
    initialCol += specular;
    
    //vec3 spotCol = ;
    
    float distance = length(lightSource.PosInCamSpc - PosInCamSpc);
    
    float attenuation = min((1.0 / 
                        (lightSource.Attenuation.x + (lightSource.Attenuation.y * distance) 
                        + lightSource.Attenuation.z * (distance * distance))), 1.0);
    
    initialCol *= attenuation;
    
    return initialCol;
}

vec3 ApplyPhongLight()
{
    vec3  finalCol;
    float attenuation;
    float SpotLight = 1.0;
    float distance = length(lightSource.PosInCamSpc - PosInCamSpc);
    
    vec3  textureCol = texture(textureData, UV).rgb;
    
    vec3  ambientCol = lightSource.AmbientColor * material.AmbientColor;
    
    if(lightSource.Type == 0)
    {
        vec3 color = PointLight(ambientCol);
        
        finalCol = color * textureCol;
        
        return finalCol;
    }
    
    if(lightSource.Type == 1)
    {
        vec3 color = DirectionalLight(ambientCol);
        
        finalCol = color * textureCol;
        
        return finalCol;
    }
    
    if(lightSource.Type == 2)
    {
        //vec3 color = SpotLight(ambientCol);
        
        //finalCol = color * textureCol;
        //
        //return finalCol;
    }
}

void main()
{   
    FragColor = vec4(ApplyPhongLight(), 1.0);
    
}