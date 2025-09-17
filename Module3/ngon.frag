#version 410 core

layout (location = 0) out vec4 frag_color;

uniform vec4 color;    // Uniform color from the application

void main()
{
    frag_color = color;
}
