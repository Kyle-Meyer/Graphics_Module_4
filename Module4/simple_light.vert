#version 410 core

// Vertex position attribute
layout (location = 0) in vec3 vtx_position;
// Vertex normal attribute
layout (location = 1) in vec3 vtx_normal;
// Color passed to the fragment shader
layout (location = 0) smooth out vec4 color;

uniform vec3 material_color; // Material diffuse color
uniform mat4 pvm_matrix;     // Composite projection, view, model matrix
uniform mat4 model_matrix;   // Composite modeling matrix
uniform mat4 normal_matrix;  // Normal transformation matrix

void main() 
{
    // Fixed light position in world coordinates. Light is behind the camera in 
    // world coordinates and is hard-coded here for now!
    vec3 light_position = vec3(0.0, -100.0, 50.0f);

    // Convert normal and position to world coords. Construct L - from vertex to light
    vec3 N = normalize(vec3(normal_matrix * vec4(vtx_normal, 0.0)));
    vec4 v = model_matrix * vec4(vtx_position, 1.0);
    vec3 L = normalize(vec3(light_position - vec3(v)));

    // The diffuse shading equation. Intnesity depends on cos of L and N
    color = vec4(material_color * max(dot(L, N), 0.0), 1.0);

    // Convert position to clip coordinates and pass along
    gl_Position = pvm_matrix * vec4(vtx_position, 1.0);
}
