#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAvgNormal;
layout(location = 4) in vec3 vTangent;
layout(location = 5) in vec3 vAvgTangent;
layout(location = 6) in vec3 vBitangent;
layout(location = 7) in vec3 vAvgBitangent;

//out variables for the fragment shader
out vec3 Vector;

//uniform variables for the transformation
uniform int Selection;
uniform mat4 m2w;
uniform mat4 m2w_normal;
uniform mat4 view;
uniform mat4 projection;
uniform int Average;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    mat4 MV = view * m2w;
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    //if average normals are used or not tranform one or the other and set it to the out variable
    if(Average == 0)
    {
        if (Selection == 0)
            Vector = vec3(normalize(vec4(mat3(m2w_normal) * vNormal, 0.0)));
        else if (Selection == 1)
            Vector = normalize(MV        * vec4(vTangent,   0.0)).xyz;
        else if (Selection == 2)
            Vector = normalize(MV        * vec4(vBitangent, 0.0)).xyz;
    }
    else
    {
        if (Selection == 0)
            Vector = vec3(normalize(vec4(mat3(m2w_normal) * vAvgNormal, 0.0)));
        else if (Selection == 1)
            Vector = normalize(MV        * vec4(vAvgTangent,   0.0)).xyz;
        else if (Selection == 2)
            Vector = normalize(MV        * vec4(vAvgBitangent, 0.0)).xyz;
    }
    
} 