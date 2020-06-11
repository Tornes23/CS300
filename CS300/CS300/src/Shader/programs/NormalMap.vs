#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAverage;
layout(location = 4) in vec3 vTangent;

//out variables for the fragment shader
out VS_OUT{
    
    vec2 UV;
    vec3 Normal;
    vec3 PosInCamSpc;
    
    vec3 Tangent;
    vec3 BitTangent;
    
    vec3 TangentFragPos;
    vec3 CamTanSpc;
    
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
    vs_out.UV = vec2(vTextCoords.x, 1 - vTextCoords.y);
    vs_out.PosInCamSpc = vec3(MV * vec4(vPos, 1.0));
    
    //if average normals are used or not tranform one or the other and set it to the out variable
    if(Average == 1)
        vs_out.Normal = vec3(normalize(vec4(mat3(m2w_normal) * vAverage, 0.0)));
    else
        vs_out.Normal = vec3(normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)));
    
    vs_out.Tangent = vec3(normalize(vec4(mat3(m2w_normal) * vTangent, 0.0)));
    
    vs_out.BitTangent = cross(vs_out.Normal, vs_out.Tangent);
    
    vs_out.TangentMat = transpose(mat3(vs_out.Tangent, vs_out.BitTangent, vs_out.Normal));
    
    vs_out.TangentFragPos = vs_out.TangentMat * vec3(gl_Position);
    
    vs_out.CamTanSpc = vs_out.TangentMat * vec3(camPosition);
    
} 