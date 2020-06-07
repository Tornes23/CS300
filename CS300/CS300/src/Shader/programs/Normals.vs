#version 400 core

//variables in the VBO
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

//structure from which to send the normal vector to the geometry shader
out VS_OUT {
    vec4 normal;
} vs_out;

//uniform variables for the transformation
uniform mat4 projection;
uniform mat4 view;
uniform mat4 m2w;
uniform mat4 m2w_normal;

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
     
    //applying the transformation to the vertex pos
    gl_Position = MVP * vec4(vPos, 1.0); 
    
    //applying the normal transformation and setting it in the struct
    vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vNormal, 0.0));
        
}