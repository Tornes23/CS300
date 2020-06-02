#version 400 core
in vec3 aPos;
  
in vec2 vTextCoords;

out vec2 UV;

uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 MVP = projection * view * m2w;
    
    gl_Position = MVP * vec4(aPos, 1.0);
    
    UV = vec2(vTextCoords.x, vTextCoords.y);
}  