
#version 400 core

layout(location = 0) in vec3 vPosition;

uniform mat4 m2w;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos; 

void main()
{
    //computing the model to projection matrix 
    mat4 MVP = projection * view * m2w;
    
	fragPos = vPosition;
    
	gl_Position =  MVP * vec4(vPosition, 1.0f);
}
