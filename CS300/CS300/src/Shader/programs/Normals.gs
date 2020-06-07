#version 400 core

//setting the primitive we get
layout (triangles) in;

//setting the number of out vertices to render
layout (line_strip, max_vertices = 6) out;

//structure from which to get the normal vector from the vertex shader
in VS_OUT {
    vec4 normal;
} gs_in[];

//setting the length of the lines
const float Length = 3;

//function to draw the lines
void DrawLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + gs_in[index].normal * Length;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    DrawLine(0); // first vertex normal
    DrawLine(1); // second vertex normal
    DrawLine(2); // third vertex normal
} 