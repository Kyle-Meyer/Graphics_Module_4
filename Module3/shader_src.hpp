#ifndef __MODULE3_SHADER_SRC_HPP__
#define __MODULE3_SHADER_SRC_HPP__

const char *ngon_vert =
    R"(
#version 410 core
layout (location = 0) in vec2 vtx_position;
uniform mat4 ortho;
void main() 
{
    gl_Position = ortho * vec4(vtx_position, 0.0, 1.0);
}
)";

const char *ngon_frag =
    R"(
#version 410 core
layout (location = 0) out vec4 frag_color;
uniform vec4 color;
void main()
{
    frag_color = color;
}
)";

const char *points_vert =
    R"(
#version 410 core
layout (location = 0) in vec2 vtx_position;
uniform mat4 ortho;
void main()
{
    gl_Position  = ortho * vec4(vtx_position, 0.0, 1.0);
}
)";

const char *points_frag =
    R"(
#version 410 core
layout (location = 0) out vec4 frag_color;
void main() 
{
    if (dot(gl_PointCoord-0.5, gl_PointCoord-0.5) > 0.25) discard;
    else frag_color = vec4(gl_PointCoord.st, 0, 1);
}
)";

const char *shaded_line_vert =
    R"(
#version 410 core
layout (location = 0) in vec2 vtx_position;
layout (location = 1) in vec4 vtx_color;  
layout (location = 0) smooth out vec4 color;
uniform mat4 ortho;
void main()
{
    color = vtx_color;
    gl_Position = ortho * vec4(vtx_position, 0.0, 1.0);
}
)";

const char *shaded_line_frag =
    R"(
#version 410 core
layout (location = 0) smooth in vec4 color;
layout (location = 0) out vec4 frag_color;
void main()
{
    frag_color = color;
}
)";

#endif
