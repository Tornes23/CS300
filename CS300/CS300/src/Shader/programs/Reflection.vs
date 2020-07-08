#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTextCoords;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vAverage;

//out variables for the fragment shader
out vec2 UV;
out vec3 CamInWorld;
out vec3 FragInWorld;
out vec3 NormalInWorld;

//uniform variables for the transformation
uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 CamPos;
uniform int Averaged;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
    FragInWorld = vec3(m2w * vec4(vPos, 1.0));
    CamInWorld  = CamPos;
    
    mat3 m2w_normal = mat3(inverse(transpose(m2w)));
    
    if(Averaged == 0)
        NormalInWorld = normalize(m2w_normal * vNormal);
    else
        NormalInWorld = normalize(m2w_normal * vAverage);
    
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0);
    
    //setting the out variable
    UV = vTextCoords;
}  