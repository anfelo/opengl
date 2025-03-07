#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    FragPos = vec3(u_model * vec4(a_pos, 1.0));
    Normal = a_normal;

    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}
