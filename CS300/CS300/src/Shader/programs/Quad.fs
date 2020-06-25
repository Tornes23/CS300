#version 400 core

in vec2 UV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D textureData;
uniform float near_plane;
uniform float far_plane;

//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // Back to NDC 
//    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
//}


void main()
{	 

    //float depthValue = texture(textureData, UV).r;
    //
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
     
    FragColor =  texture(textureData, UV).rgba;
}