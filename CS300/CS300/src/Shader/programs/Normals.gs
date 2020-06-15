#version 400 core

//setting the primitive we get
layout (triangles) in;

//setting the number of out vertices to render
layout (line_strip, max_vertices = 18) out;

//structure from which to get the normal vector from the vertex shader
in VS_OUT {
    vec4 normal;
    vec4 tangent;
    vec4 bitangent;
    
    vec3 normalColor;
    vec3 tanColor;
    vec3 bitanColor;
    
} gs_in[];

out vec3 lineColor;

//setting the length of the lines
const float Length = 3;

//function to draw the lines
void DrawLine(int index, vec4 vector, vec3 color)
{
    gl_Position = gl_in[index].gl_Position;
    lineColor = color;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vector * Length;
    lineColor = color;
    EmitVertex();
    EndPrimitive();
}

void DrawNormals()
{
    DrawLine(0, gs_in[0].normal, gs_in[0].normalColor); // first vertex normal
    DrawLine(1, gs_in[1].normal, gs_in[1].normalColor); // second vertex normal
    DrawLine(2, gs_in[2].normal, gs_in[2].normalColor); // third vertex normal
} 

void DrawTangents()
{
    DrawLine(0, gs_in[0].tangent, gs_in[0].tanColor); // first vertex normal
    DrawLine(1, gs_in[1].tangent, gs_in[1].tanColor); // second vertex normal
    DrawLine(2, gs_in[2].tangent, gs_in[2].tanColor); // third vertex normal
} 

void DrawBitangents()
{
    DrawLine(0, gs_in[0].bitangent, gs_in[0].bitanColor); // first vertex normal
    DrawLine(1, gs_in[1].bitangent, gs_in[1].bitanColor); // second vertex normal
    DrawLine(2, gs_in[2].bitangent, gs_in[2].bitanColor); // third vertex normal
} 
void main()
{
    DrawNormals();
    DrawTangents();
    DrawBitangents();
    //DrawLine(0); // first vertex normal
    //DrawLine(1); // second vertex normal
    //DrawLine(2); // third vertex normal
} 