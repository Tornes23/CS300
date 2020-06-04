#version 400 core

in vec3 vPos;
in vec2 vTexcoords;
in vec3 vNormal;
in vec3 vAveraged;

out VS_OUT {
    vec4 normal;
} vs_out;

out vec2 UV;
flat out int AvNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 m2w;
uniform mat4 m2w_normal;
uniform int Averaged;

void main()
{
    mat4 MVP = projection * view * m2w;
     
    UV = vec2(vTexcoords.x, 1 - vTexcoords.y);
     
    gl_Position = MVP * vec4(vPos, 1.0); 
    
    if(Averaged == 1)
        vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vAveraged, 0.0));
    else
        vs_out.normal = projection * normalize(vec4(mat3(m2w_normal) * vNormal, 0.0));
        
    AvNormal = Averaged;
        
}