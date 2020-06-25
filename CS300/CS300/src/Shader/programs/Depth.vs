#version 400

//variable in the VBO
layout(location = 0) in vec3 vPos;

uniform mat4 lightSpace;
uniform mat4 m2w;

void main()
{
   //setting the position
   gl_Position = lightSpace * m2w * vec4(vPos, 1.0F);
}