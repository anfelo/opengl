#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_image;
uniform vec3 u_sprite_color;

void main()
{
    FragColor = vec4(u_sprite_color, 1.0) * texture(u_image, TexCoords);
}
