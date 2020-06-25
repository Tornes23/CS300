#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAverage;

//out variables for the fragment shader
out vec2 UV;
out vec3 Normal;
out vec3 PosInCamSpc;

//uniform variables for the transformation
uniform int Average;
uniform mat4 m2w;
uniform mat4 m2w_normal;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
    //computing the model to view matrix 
    mat4 MV = view * m2w;
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    //setting the out variables
    UV = vTextCoords;
    
    PosInCamSpc = vec3(MV * vec4(vPos, 1.0));
    
    //if average normals are used or not tranform one or the other and set it to the out variable
    if(Average == 1)
        Normal = normalize(vec4(mat3(m2w_normal) * vAverage, 0.0)).xyz;
    else
        Normal = normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)).xyz;
    
} 