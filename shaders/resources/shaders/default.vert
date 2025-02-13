#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;

uniform float u_scale;

out vec3 color;

void main()
{
   gl_Position = vec4(a_pos.x * (1 + u_scale), a_pos.y * (1 + u_scale), a_pos.z * (1 + u_scale), 1.0);
   color = a_color;
}
