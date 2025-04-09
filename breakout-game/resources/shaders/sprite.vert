#version 330 core
layout (location = 0) in vec4 a_vertex;

uniform mat4 u_model;
uniform mat4 u_projection;

out vec2 TexCoords;

void main()
{
    TexCoords = a_vertex.zw;
    gl_Position = u_projection * u_model * vec4(a_vertex.xy, 0.0, 1.0);
    //gl_Position = vec4(a_vertex.xy, 0.0, 1.0);
}
