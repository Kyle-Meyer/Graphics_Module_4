#version 410 core

// Smooth interpolation of color - input from vertex shader
layout (location = 0) smooth in vec4 color;
// Output fragment color
layout (location = 0) out vec4 frag_color;

void main()
{
    frag_color = color;
}
