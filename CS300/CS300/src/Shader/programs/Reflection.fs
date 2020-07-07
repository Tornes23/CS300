#version 400 core

in vec2 UV;
in vec3 CamInWorld;
in vec3 FragInWorld;
in vec3 NormalInWorld;

out vec4 FragColor;

uniform sampler2D textureData;

uniform samplerCube EnviromentMap;

uniform int Mode;

uniform float RefractionVal;

void main()
{
    vec3 viewVector = normalize(FragInWorld - CamInWorld);
    
    vec3 reflectedVec = reflect(viewVector, NormalInWorld);
    
    vec3 refractedVec = refract(viewVector, NormalInWorld, RefractionVal);
    
    if(Mode == 0)
        FragColor = texture(textureData, UV).rgba;
    
    if(Mode == 1)
        FragColor = texture(EnviromentMap, normalize(reflectedVec)).rgba;
    
    if(Mode == 2)
        FragColor = texture(EnviromentMap, normalize(refractedVec)).rgba;
}