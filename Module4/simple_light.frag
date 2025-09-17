#version 410 core

// Incoming color - interpolated
layout (location = 0) smooth in vec4 color;
layout (location = 0) out vec4 frag_color;

void main() 
{
    frag_color = color;
}
