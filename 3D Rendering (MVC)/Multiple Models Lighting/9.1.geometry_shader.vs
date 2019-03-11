#version 330 core
layout (location = 0) in vec4 aPos;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout (location = 3) in mat4 instanceMatrix;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;


// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat4 P;
uniform vec3 LightPosition_worldspace;

out VS_OUT {
    float radius;
} vs_out;

void main()
{
    vs_out.radius = aPos.w;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
}