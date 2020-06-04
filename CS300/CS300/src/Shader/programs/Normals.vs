#version 400 core

in vec3 vPos;
in vec2 vTexcoords;
in vec3 vNormal;
in vec3 vAveraged;

out VS_OUT {
    vec3 normal;
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
    
    //gl_Position = MVP * vec4(vPos, 1.0);
    
     //gl_Position = projection * view * m2w * vec4(aPos, 1.0); 
     
    UV = vec2(vTexcoords.x, 1 - vTexcoords.y);
     
    gl_Position = MVP * vec4(vPos, 1.0); 
    
    if(Averaged == 1)
        vs_out.normal = normalize(vec3(m2w_normal * vec4(vAveraged, 0.0)));
    else
        vs_out.normal = normalize(vec3(m2w_normal * vec4(vNormal, 1.0)));
        
    AvNormal = Averaged;
        
}