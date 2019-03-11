#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

in VS_OUT {
    float radius;
} gs_in[];


void build_house(vec4 position)
{    
    gl_Position = position;// + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left
    gl_PointSize = 1.0f;   
    EmitVertex();   
    
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}
