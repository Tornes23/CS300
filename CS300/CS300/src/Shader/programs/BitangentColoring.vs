#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAverage;

//out variables for the fragment shader
out vec2 UV;
out vec3 Tangent;

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
    
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    //if average normals are used or not tranform one or the other and set it to the out variable
    if(Average == 1)
        Normal = vec3(normalize(vec4(mat3(m2w_normal) * vAverage, 0.0)));
    else
        Normal = vec3(normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)));
    
} 