#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vAverage;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vAvgTangent;
layout(location = 5) in vec3 vBitangent;
layout(location = 6) in vec3 vAvgBitangent;

//structure from which to send the normal vector to the geometry shader
out VS_OUT {
    vec4 normal;
    vec4 tangent;
    vec4 bitangent;
    
    vec3 normalColor;
    vec3 tanColor;
    vec3 bitanColor;
} vs_out;

//uniform variables for the transformation
uniform mat4 projection;
uniform mat4 view;
uniform mat4 m2w;
uniform mat4 m2w_normal;
uniform int Average;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
     
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0); 
    
    //applying the normal transformation and setting it in the struct
    if(Average == 0)
    {
        vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vNormal, 0.0));
        vs_out.normalColor = vec3(0, 0, 1);
        vs_out.tangent = normalize(MVP * vec4(vTangent, 0.0));
        vs_out.tanColor = vec3(1, 0, 0);
        vs_out.bitangent = normalize(MVP * vec4(vBitangent, 0.0));
        vs_out.bitanColor = vec3(0, 1, 0);
    }
    else
    {
        vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vAverage, 0.0));
        vs_out.normalColor = vec3(0, 0, 1);
        vs_out.tangent = normalize(MVP * vec4(vAvgTangent, 0.0));
        vs_out.tanColor = vec3(1, 0, 0);
        vs_out.bitangent = normalize(MVP * vec4(vAvgBitangent, 0.0));
        vs_out.bitanColor = vec3(0, 1, 0);
    }
    
   
        
}