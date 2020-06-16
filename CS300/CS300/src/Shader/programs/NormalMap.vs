#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAverage;
layout(location = 4) in vec3 vTangent;
layout(location = 5) in vec3 vBitangent;

//out variables for the fragment shader
out VS_OUT{
    
    vec2 UV;
    vec3 Normal;
    vec3 PosInCamSpc;
    
    vec3 Tangent;
    vec3 BitTangent;
    
    mat3 TangentMat;
    
} vs_out;

//uniform variables for the transformation
uniform int Average;
uniform mat4 m2w;
uniform mat4 m2w_normal;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPosition;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
    //computing the model to view matrix 
    mat4 MV = view * m2w;
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    //setting the out variables
    vs_out.UV = vTextCoords;
    vs_out.PosInCamSpc = (MV * vec4(vPos, 1.0)).xyz;
    
    //if average normals are used or not tranform one or the other and set it to the out variable
    if(Average == 1)
        vs_out.Normal = mat3(m2w_normal) * vAverage;
    else
        vs_out.Normal = mat3(m2w_normal) * vNormal;
    
    //computing the tangent in camera space
    //computing the bitangent in camera space
    
    vs_out.Tangent = mat3(MV) * vTangent;
    vs_out.BitTangent =  mat3(MV) * vBitangent;
    
    //computing the tangent matrix 
    vs_out.TangentMat = mat3(normalize(vs_out.Tangent), normalize(vs_out.BitTangent), normalize(vs_out.Normal));
    
} 