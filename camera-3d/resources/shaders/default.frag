#version 330 core
out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float u_tex;

void main()
{
    FragColor = mix(texture(texture1, tex_coord), texture(texture2, vec2(1.0 - tex_coord.x, tex_coord.y)), u_tex);
}
