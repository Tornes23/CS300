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

uniform int UseTexture;

//output color
out vec4 FragColor; 

//in variables we get from the vertex shader
in vec3 PosInCamSpc; 
in vec2 UV;
in vec3 Normal;
in vec4 FragLightSpc;

//uniform variable to get the data of the texture
uniform sampler2D textureData;
uniform sampler2D shadowMap;

//float ComputePCFShadow()
//{
//    
//}

float ComputeShadow()
{
    // perform perspective divide
    vec3 projCoords = FragLightSpc.xyz / FragLightSpc.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}


vec3 SpotLight(vec3 initialCol, int i, float shadowVal)
{
    //computing the distance
    float distance = length(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing the light direction
    vec3 lightDir = normalize(lightSources[i].PosInCamSpc - PosInCamSpc);
    
    //computing diffuse value and color
    float diffuseVal = max(dot(Normal, lightDir), 0.0);
    vec3 diffuseCol = (diffuseVal * material.DiffuseColor) * lightSources[i].DiffuseColor;
    
    //computing specular color
    vec3 viewDir = normalize(-PosInCamSpc);//since im cam space cam pos = origin

    //computing the reflection direction
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    //computing the speculat factor and color
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = (material.SpecularColor * spec) * lightSources[i].SpecularColor;  
    
    //adding it to the color
    initialCol += (shadowVal * (diffuseCol + specular));
    
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
        vec3  textureCol;
        if(UseTexture == 1)
            textureCol = texture(textureData, vec2(UV.x, 1 - UV.y)).rgb;
        else
            textureCol = vec3(UV, 0.0F);
        
        //computing the ambient color
        vec3  ambientCol = lightSources[i].AmbientColor * material.AmbientColor;
        
        //variable to store the color after the lighting process
        vec3 color;
        
        //variable to store the shadow value
        float shadow = ComputeShadow();
            
        color = SpotLight(ambientCol, i, shadow);
        
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