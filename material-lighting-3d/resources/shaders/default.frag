#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_light;
uniform Material u_material;
uniform vec3 u_view_pos;

void main()
{
    // ambient
    vec3 ambient = u_light.ambient * u_material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(u_light.position - FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);

    // specular
    vec3 view_dir = normalize(u_view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * (spec * u_material.specular);

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
