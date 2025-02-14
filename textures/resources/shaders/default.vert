#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_tex_coord;

uniform float u_scale;

out vec2 tex_coord;

void main()
{
    gl_Position = vec4(a_pos.x * (1 + u_scale), a_pos.y * (1 + u_scale), a_pos.z * (1 + u_scale), 1.0);
    tex_coord = a_tex_coord;
}
