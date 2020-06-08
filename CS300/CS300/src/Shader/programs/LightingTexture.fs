#version 400 core

//Light structure
struct Light{
    
    int Type;

    vec3 Position;
    vec3 Direction;
    vec3 PosInCamSpc;
    
    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    vec3 Attenuation;
    
    float CosInner;
    float CosOuter;
    float FallOff;
    
};

//Material structure
struct Material{

    vec3 AmbientColor;
    vec3 DiffuseColor;
    vec3 SpecularColor;
    
    float Shininess;
    
};

//uniform variable to store all the lights
uniform Light lightSources[8];

//uniform variable to store the number ofg lights
uniform int lightCount; 

//uniform variable to store the material
uniform Material material;

//output color
out vec4 FragColor; 

//in variables we get from the vertex shader
in vec3 PosInCamSpc; 
in vec2 UV;
in vec3 Normal;

//uniform variable to get the data of the texture
uniform sampler2D textureData;

vec3 PointLight(vec3 initialCol, int i)
{
    //computing the distance
    float distance = length(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing the light direction
    vec3 lightDir = normalize(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing diffuse value and color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    //adding it to the color
    initialCol += diffuseCol;
    
    //computing specular color
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    //computing the reflection direction
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    //computing the speculat factor and color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    //adding it to the color
    initialCol += specular;
    
    //computing the attenuation
    float attenuation = min((1.0 / 
                        (lightSources[i].Attenuation.x + (lightSources[i].Attenuation.y * distance) 
                        + lightSources[i].Attenuation.z * (distance * distance))), 1.0);
                        
    //applying the attenuation to the color     
    initialCol *= attenuation;
    
    //return the color after the lighting
    return initialCol;
}

vec3 DirectionalLight(vec3 initialCol, int i)
{   
    //computing the light direction
    vec3 lightDir = normalize(-lightSources[i].Direction);
    
    //computing diffuse value and color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    //adding it to the color
    initialCol += diffuseCol;
    
    //computing specular color
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin
    
    //computing the reflection direction
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    //computing the speculat factor and color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    //adding it to the color
    initialCol += specular;
    
    //return the color after the lighting
    return initialCol; 
}

vec3 SpotLight(vec3 initialCol, int i)
{
    //computing the distance
    float distance = length(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing the light direction
    vec3 lightDir = normalize(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing diffuse value and color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    //adding it to the color
    initialCol += diffuseCol;
    
    //computing specular color
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    //computing the reflection direction
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    //computing the speculat factor and color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    //adding it to the color
    initialCol += specular;
    
    //computing the attenuation
    float attenuation = min((1.0 / 
                        (lightSources[i].Attenuation.x + (lightSources[i].Attenuation.y * distance) 
                        + lightSources[i].Attenuation.z * (distance * distance))), 1.0);
                        
    initialCol *= attenuation;
    
    //computing the spotLight factor
    float alphaCosine = dot(normalize(lightSources[i].Direction), -lightDir);
    
    float spotLight = pow((alphaCosine - lightSources[i].CosOuter) / 
                        (lightSources[i].CosInner - lightSources[i].CosOuter), lightSources[i].FallOff);
             
    //applying it to the color
    initialCol *= clamp(spotLight, 0.0F, 1.0F);
    
    //return the color after the lighting
    return initialCol;
}

vec3 ApplyPhongLight()
{
    //variable to store the final color
    vec3  finalCol;
    
    //for the number of lights in the level
    for(int i = 0; i < lightCount; i++)
    {
        //get the texture color
        vec3  textureCol = texture(textureData, UV).rgb;
        
        //computing the ambient color
        vec3  ambientCol = lightSources[i].AmbientColor * material.AmbientColor;
        
        //variable to store the color after the lighting process
        vec3 color;
 
        //if the light type is point light
        if(lightSources[i].Type == 0)
            color = PointLight(ambientCol, i);
        
        //if the light type is directional light
        if(lightSources[i].Type == 1)
            color = DirectionalLight(ambientCol, i);
        
        //if the light type is SpotLight
        if(lightSources[i].Type == 2)
            color = SpotLight(ambientCol, i);
        
        //computing the final color
        finalCol += color * textureCol;
    }
    
    //returning the final color
    return finalCol;
}

void main()
{   
    //setting the final color as the noe returned after aplying lights
    FragColor = vec4(ApplyPhongLight(), 1.0);
    
}