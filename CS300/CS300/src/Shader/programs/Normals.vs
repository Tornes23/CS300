#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vAverage;

//structure from which to send the normal vector to the geometry shader
out VS_OUT {
    vec4 normal;
    
    vec3 normalColor;
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
        vs_out.normalColor = vec3(1,1,1);
    }
    else
    {
        vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vAverage, 0.0));
        vs_out.normalColor = vec3(1,0,0);
    }
    
        
}