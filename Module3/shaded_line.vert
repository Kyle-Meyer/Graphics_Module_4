#version 410 core

// Vertex position attribute
layout (location = 0) in vec2 vtx_position;
// Vertex color attribute
layout (location = 1) in vec4 vtx_color;  
// Smooth interpolation of color output to fragment shader 
layout (location = 0) smooth out vec4 color;

// Uniform transformation matrix from application
uniform mat4 ortho;

void main()
{
    color = vtx_color;
    gl_Position = ortho * vec4(vtx_position, 0.0, 1.0);
}
