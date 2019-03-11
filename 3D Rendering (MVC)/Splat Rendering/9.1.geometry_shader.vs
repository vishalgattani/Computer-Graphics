#version 330 core
layout (location = 0) in vec4 aPos;



out VS_OUT {
    float radius;
} vs_out;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat4 P;
uniform vec3 LightPosition_worldspace;

void main()
{
    vs_out.radius = aPos.w;
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
}