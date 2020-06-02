#version 400 core

in vec3 aPos;
in vec2 vTextCoords;
in vec3 vNormal;

out vec3 Pos;
out vec2 UV;
out vec3 Normal;

uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 MVP = projection * view * m2w;
    
    Pos = vec3(m2w * vec4(aPos, 1.0));
    UV = vec2(vTextCoords.x, 1 - vTextCoords.y);
    Normal = mat3(transpose(inverse(m2w))) * vNormal;
    
    gl_Position = MVP * vec4(aPos, 1.0);
}  