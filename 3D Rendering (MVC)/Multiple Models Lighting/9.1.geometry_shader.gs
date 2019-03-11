#version 330 core
layout (points) in;
layout (triangle_strip,max_vertices=3) out;

in VS_OUT {
    float radius;
} gs_in[];

void build_house(vec4 position)
{    
    gl_Position = position + vec4(gs_in[0].radius, 0.0, 0.0, 0.0);   
    EmitVertex();   
    gl_Position = position + vec4(0.0 ,gs_in[0].radius, 0.0, 0.0);   
    EmitVertex();   
    gl_Position = position + vec4(0.0, 0.0, gs_in[0].radius, 0.0);   
    EmitVertex();   
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}