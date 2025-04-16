#version 330 core
layout (location = 0) in vec4 vertex;

uniform mat4 u_model;
uniform mat4 u_projection;

out vec2 TexCoords;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = u_projection * u_model * vec4(vertex.xy, 0.0, 1.0);
}
