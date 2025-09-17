#version 410 core

layout (location = 0) in vec2 vtx_position;

uniform mat4 ortho;

void main() 
{
    gl_Position = ortho * vec4(vtx_position, 0.0, 1.0);
}
